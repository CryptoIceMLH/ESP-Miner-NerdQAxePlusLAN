#include <inttypes.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

#include "esp_log.h"
#include "driver/gpio.h"
#include "esp_app_desc.h"
#include "esp_netif.h"
#include "esp_timer.h"
#include "esp_wifi.h"
#include "lwip/inet.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "connect.h"

#include "global_state.h"
#include "displays/displayDriver.h"
#include "system.h"
#include "i2c_master.h"
#include "nvs_config.h"
#include "influx_task.h"
#include "history.h"
#include "boards/board.h"
#include "utils.h"

#ifdef CONFIG_ENABLE_ETHERNET
extern "C" {
    #include "ethernet_w5500.h"
}
#endif

static const char* TAG = "SystemModule";

System::System() : m_networkMode(NETWORK_MODE_WIFI), m_ethAvailable(false),
                   m_ethLinkUp(false), m_ethConnected(false) {
    // Initialize network state to safe defaults
    // setup_network() in main.cpp will override these if Ethernet hardware is detected
    strcpy(m_ethMacAddress, "00:00:00:00:00:00");  // Default, will be set by setup_network()
}

void System::initSystem() {
    m_screenPage = 0;
    m_startTime = esp_timer_get_time();
    m_startupDone = false;

    // Initialize overheat flag
    m_boardError = Board::Error::NONE;

    // Initialize shown overlay flag and last error code
    m_showsOverlay = false;
    m_currentErrorCode = 0;

    // Clear the ssid string
    memset(m_ssid, 0, sizeof(m_ssid));

    // Clear the wifi_status string
    memset(m_wifiStatus, 0, 20);

    // initialize AP state
    m_apState = false;

    // Initialize Ethernet state
    // NOTE: Do NOT reset m_networkMode, m_ethAvailable, or m_ethMacAddress here - they're set by
    // setup_network() in main.cpp before this task starts. Resetting them would overwrite values.
    // Only reset IP to default (will be updated by updateEthernetStatus()):
    strcpy(m_ethIpAddress, "0.0.0.0");

    // Initialize the display
    m_display = new DisplayDriver();
    m_display->loadSettings();
    m_display->init(m_board);

    m_netif = esp_netif_get_handle_from_ifkey("WIFI_STA_DEF");

    m_hostname = Config::getHostname();

    m_history = new History();
    if (!m_history->init(m_board->getAsicCount())) {
        ESP_LOGE(TAG, "history couldn't be initialized!");
    }
}

void System::loadSettings() {
    if (m_display) {
        m_display->loadSettings();
    }
}

void System::updateHashrate() {}
void System::updateBestDiff() {}
void System::clearDisplay() {}
void System::updateSystemInfo() {}
void System::updateEsp32Info() {}
void System::initConnection() {}

void System::updateConnection() {
    // Update status text based on network mode
    if (m_networkMode == NETWORK_MODE_ETHERNET) {
        // Show "LAN" or Ethernet connection status instead of WiFi
#ifdef CONFIG_ENABLE_ETHERNET
        if (m_ethConnected) {
            m_display->updateWifiStatus("LAN");
        } else {
            m_display->updateWifiStatus("LAN OFF");
        }
#else
        m_display->updateWifiStatus("LAN");
#endif
    } else {
        // Show WiFi status
        m_display->updateWifiStatus(m_wifiStatus);
    }
}

void System::updateSystemPerformance() {}

void System::showApInformation(const char* error) {
    char apSsid[13];
    generate_ssid(apSsid);
    m_display->portalScreen(apSsid);
}

const char* System::getMacAddress() {
    return connect_get_mac_addr();
}

// Function to fetch and return the RSSI (dBm) value
int System::get_wifi_rssi()
{
    // Skip RSSI check if using Ethernet mode
    if (m_networkMode == NETWORK_MODE_ETHERNET) {
        ESP_LOGD("WIFI_RSSI", "Using Ethernet mode, RSSI not applicable");
        return 0;  // Return 0 to indicate Ethernet mode (no WiFi signal)
    }

    wifi_ap_record_t ap_info;

    // Query the connected Access Point's information
    if (esp_wifi_sta_get_ap_info(&ap_info) == ESP_OK) {
        ESP_LOGI("WIFI_RSSI", "Current RSSI: %d dBm", ap_info.rssi);
        return ap_info.rssi;  // Return the actual RSSI value
    } else {
        ESP_LOGE("WIFI_RSSI", "Failed to fetch RSSI");
        return -90;  // Return -90 to indicate an error
    }
}

float System::getCurrentHashrate() {
    if (m_board->hasHashrateCounter()) {
        return HASHRATE_MONITOR.getSmoothedTotalChipHashrate();
    }
    return getCurrentHashrate10m();
}

esp_reset_reason_t System::showLastResetReason() {
    esp_reset_reason_t reason = esp_reset_reason();
    switch (reason) {
        case ESP_RST_UNKNOWN: m_lastResetReason = "SYSTEM.RESET_UNKNOWN"; break;
        case ESP_RST_POWERON: m_lastResetReason = "SYSTEM.RESET_POWERON"; break;
        case ESP_RST_EXT: m_lastResetReason = "SYSTEM.RESET_EXTERNAL"; break;
        case ESP_RST_SW: m_lastResetReason = "SYSTEM.RESET_SOFTWARE"; break;
        case ESP_RST_PANIC: m_lastResetReason = "SYSTEM.RESET_PANIC"; break;
        case ESP_RST_INT_WDT: m_lastResetReason = "SYSTEM.RESET_INT_WATCHDOG"; break;
        case ESP_RST_TASK_WDT: m_lastResetReason = "SYSTEM.RESET_TASK_WATCHDOG"; break;
        case ESP_RST_WDT: m_lastResetReason = "SYSTEM.RESET_OTHER_WATCHDOG"; break;
        case ESP_RST_DEEPSLEEP: m_lastResetReason = "SYSTEM.RESET_DEEPSLEEP"; break;
        case ESP_RST_BROWNOUT: m_lastResetReason = "SYSTEM.RESET_BROWNOUT"; break;
        case ESP_RST_SDIO: m_lastResetReason = "SYSTEM.RESET_SDIO"; break;
        default: m_lastResetReason = "SYSTEM.RESET_NOT_SPECIFIED"; break;
    }
    ESP_LOGI(TAG, "Reset reason: %s", m_lastResetReason);
    return reason;
}

void System::showError(const char *error_message, uint32_t error_code) {
    // is this error already shown? yes, do nothing
    if (m_showsOverlay && m_currentErrorCode == error_code) {
        return;
    }
    m_display->showError(error_message, error_code);
    m_showsOverlay = true;
    m_currentErrorCode = error_code;
}

void System::hideError() {
    if (!m_showsOverlay) {
        return;
    }
    m_display->hideError();
    m_currentErrorCode = 0;
    m_showsOverlay = false;
}

void System::taskWrapper(void* pvParameters) {
    System* systemInstance = static_cast<System*>(pvParameters);
    systemInstance->task();
}

void System::task() {
    initSystem();
    clearDisplay();
    initConnection();

    ESP_LOGI(TAG, "SYSTEM_task started");

    // wait until splash1 and splash2 timed out
    m_display->waitForSplashs();

    wifi_mode_t wifiMode;
    esp_err_t result;

    while (!m_startupDone) {
        // Check if STA has a valid IP
        char ip[20] = {0};
        bool sta_has_ip = connect_get_ip_addr(ip, sizeof(ip)); // returns true if ip_valid

        // Check whether AP is active
        result = esp_wifi_get_mode(&wifiMode);
        bool ap_active = (result == ESP_OK) && (wifiMode == WIFI_MODE_AP || wifiMode == WIFI_MODE_APSTA);

        if (!sta_has_ip && ap_active) {
            // STA not connected yet -> show captive/config info
            showApInformation(nullptr);
            vTaskDelay(pdMS_TO_TICKS(5000)); // avoid flicker/spam
        } else {
            // Either STA is connected or AP is off -> show normal connection UI
            updateConnection();
            vTaskDelay(pdMS_TO_TICKS(100));
        }
    }

    m_display->miningScreen();

    char lastIpAddress[20] = {0};

    // show initial 0.0.0.0
    m_display->updateIpAddress(m_ipAddress);
    int lastFoundBlocks = 0;

    int toggle = 1;

    while (1) {
        if (POWER_MANAGEMENT_MODULE.isShutdown()) {
            ESP_LOGW(TAG, "suspended");
            vTaskSuspend(NULL);
        }
        // update Ethernet status if available
#ifdef CONFIG_ENABLE_ETHERNET
        updateEthernetStatus();  // This updates m_ipAddress for Ethernet mode
#endif

        // update IP on the screen if it is available
        // WiFi mode: get IP from WiFi
        // Ethernet mode: already updated by updateEthernetStatus() above
        if (m_networkMode == NETWORK_MODE_WIFI) {
            if (connect_get_ip_addr(m_ipAddress, sizeof(m_ipAddress))) {
                if (strcmp(m_ipAddress, lastIpAddress) != 0) {
                    ESP_LOGI(TAG, "WiFi IP address: %s", m_ipAddress);
                    m_display->updateIpAddress(m_ipAddress);
                    strncpy(lastIpAddress, m_ipAddress, sizeof(lastIpAddress));
                }
            }
        } else if (m_networkMode == NETWORK_MODE_ETHERNET) {
            // For Ethernet, m_ipAddress was already updated by updateEthernetStatus()
            if (strcmp(m_ipAddress, lastIpAddress) != 0) {
                ESP_LOGI(TAG, "Ethernet IP address changed: '%s' -> '%s'", lastIpAddress, m_ipAddress);
                m_display->updateIpAddress(m_ipAddress);
                strncpy(lastIpAddress, m_ipAddress, sizeof(lastIpAddress));
                lastIpAddress[sizeof(lastIpAddress) - 1] = '\0';
            }
        }

        if (m_boardError != Board::Error::NONE) {
            showError(Board::errorToStr(m_boardError), m_errorCode);
        }

        uint32_t foundBlocks = STRATUM_MANAGER->getFoundBlocks();

        // trigger the overlay only once when block is found
        if (foundBlocks != lastFoundBlocks && foundBlocks) {
            m_display->showFoundBlockOverlay();
        }
        lastFoundBlocks = foundBlocks;

        // toggle
        toggle = 1-toggle;

        m_display->updateGlobalState(toggle);
        m_display->updateCurrentSettings(toggle);
        m_display->refreshScreen();

        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}

void System::notifyMiningStarted() {}

void System::notifyAcceptedShare() {}

void System::notifyRejectedShare() {}

#ifdef CONFIG_ENABLE_ETHERNET
#include "connect.h"

void System::updateEthernetStatus() {
    // Only update if hardware available AND in Ethernet mode
    if (!m_ethAvailable || m_networkMode != NETWORK_MODE_ETHERNET) {
        return;
    }

    // Get IP address
    char ip_buf[IP4ADDR_STRLEN_MAX];
    if (ethernet_get_ip(ip_buf, sizeof(ip_buf))) {
        m_ethConnected = true;
        strncpy(m_ethIpAddress, ip_buf, IP4ADDR_STRLEN_MAX - 1);
        m_ethIpAddress[IP4ADDR_STRLEN_MAX - 1] = '\0';

        // Sync global IP if in Ethernet mode
        if (m_networkMode == NETWORK_MODE_ETHERNET) {
            strncpy(m_ipAddress, m_ethIpAddress, IP4ADDR_STRLEN_MAX - 1);
            m_ipAddress[IP4ADDR_STRLEN_MAX - 1] = '\0';
        }
    } else {
        m_ethConnected = false;
    }

    // Get actual PHY link status from W5500
    m_ethLinkUp = ethernet_w5500_get_link_status();

    // Get MAC address
    char mac_buf[18];
    if (ethernet_get_mac(mac_buf, sizeof(mac_buf))) {
        strncpy(m_ethMacAddress, mac_buf, sizeof(m_ethMacAddress) - 1);
        m_ethMacAddress[sizeof(m_ethMacAddress) - 1] = '\0';
    }
}
#endif // CONFIG_ENABLE_ETHERNET


