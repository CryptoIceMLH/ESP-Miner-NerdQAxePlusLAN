# Ethernet/LAN Support Implementation for NerdQAxePlus Miner

**Version**: 3.1
**Date**: 2025-01-14
**Author**: MLH BTC
**Target Platform**: ESP32-S3 with ESP-IDF v5.3.x
**Hardware**: W5500 SPI Ethernet Controller

---

## Executive Summary

This document describes the complete implementation of Ethernet/LAN capability for the NerdQAxePlus Bitcoin miner firmware using the WIZnet W5500 SPI Ethernet controller. The implementation adds dual network support (WiFi + Ethernet) with automatic fallback mechanisms while maintaining full backward compatibility with WiFi-only configurations.

### Key Features
- ✅ Full W5500 SPI Ethernet driver using ESP-IDF native APIs
- ✅ Dual network mode support (WiFi or Ethernet, user selectable)
- ✅ Automatic WiFi fallback on Ethernet failure
- ✅ Complete web UI integration for network configuration
- ✅ NVS-based runtime configuration persistence
- ✅ Backward compatible (WiFi-only builds unaffected)
- ✅ Professional ESP-IDF patterns with comprehensive error handling

---

## Table of Contents

1. [Architecture Overview](#1-architecture-overview)
2. [New Components](#2-new-components)
3. [Core File Modifications](#3-core-file-modifications)
4. [Network Initialization Flow](#4-network-initialization-flow)
5. [API Changes](#5-api-changes)
6. [Configuration Options](#6-configuration-options)
7. [Build System Changes](#7-build-system-changes)
8. [Testing & Validation](#8-testing--validation)
9. [Migration Guide](#9-migration-guide)
10. [Troubleshooting](#10-troubleshooting)

---

## 1. Architecture Overview

### 1.1 Design Philosophy

The Ethernet implementation follows these core principles:

1. **Single Initialization**: Network infrastructure (esp_netif + event_loop) initialized once before any interface
2. **Priority Fallback**: Ethernet attempted first (if selected), automatic WiFi fallback on failure
3. **Event-Driven**: Uses ESP-IDF event system for network state management
4. **Zero Impact**: WiFi-only builds remain unaffected (conditional compilation)
5. **User Choice**: Runtime network mode selection via NVS configuration

### 1.2 Network Mode Flow

```
┌─────────────────────────────────────────────────────────────┐
│                    System Boot Sequence                      │
└─────────────────────────────────────────────────────────────┘
                            │
                            ▼
    ┌──────────────────────────────────────────────┐
    │  Initialize Network Infrastructure ONCE      │
    │  (esp_netif_init + event_loop)              │
    └──────────────────────────────────────────────┘
                            │
                            ▼
    ┌──────────────────────────────────────────────┐
    │  Read NVS: network_mode = "wifi"/"ethernet"  │
    └──────────────────────────────────────────────┘
                            │
              ┌─────────────┴─────────────┐
              │                           │
         "ethernet"                     "wifi"
              │                           │
              ▼                           ▼
    ┌─────────────────────┐    ┌─────────────────────┐
    │ Initialize W5500    │    │  Initialize WiFi    │
    │ Wait 10s for DHCP   │    │  Start STA + AP     │
    └─────────────────────┘    └─────────────────────┘
              │                           │
      ┌───────┴──────┐                   │
      │              │                   │
   Success      Timeout/Fail             │
      │              │                   │
      │              └───────────────────┘
      │                           │
      │                           ▼
      │              ┌────────────────────────┐
      │              │ WiFi Fallback Mode     │
      │              │ (AP for config portal) │
      │              └────────────────────────┘
      │                           │
      └───────────────────────────┘
                  │
                  ▼
      ┌────────────────────┐
      │  Network Ready     │
      │  Start HTTP Server │
      │  Continue Boot     │
      └────────────────────┘
```

---

## 2. New Components

### 2.1 Ethernet W5500 Driver (`components/ethernet_w5500/`)

Complete ESP-IDF native Ethernet driver implementation.

#### File Structure
```
components/ethernet_w5500/
├── CMakeLists.txt                 # Component registration
├── Kconfig.projbuild              # Build-time configuration
├── ethernet_w5500.c               # Main driver implementation (325 lines)
└── include/
    ├── ethernet_w5500.h           # Public API header
    ├── w5500.h                    # W5500 register definitions
    ├── w5500_config.h             # WIZnet chip selection
    └── wizchip_conf.h             # WIZnet ioLibrary configuration
```

#### Key Functions

##### Initialization
```c
esp_err_t ethernet_w5500_init(void);
```
- Initializes SPI bus (GPIO 12/16/2/21)
- 100ms stabilization delay for W5500 PLL
- Generates unique MAC address from ESP32 chip ID
- Creates ESP-IDF MAC/PHY instances
- Registers event handlers (ETH_EVENT, IP_EVENT)
- Starts Ethernet driver

##### Status Queries
```c
bool ethernet_w5500_is_available(void);    // Hardware detected
bool ethernet_w5500_is_connected(void);    // Has IP address
bool ethernet_w5500_get_link_status(void); // PHY link up
```

##### Information Retrieval
```c
esp_err_t ethernet_w5500_get_ip(char *ip_str, size_t len);
esp_err_t ethernet_w5500_get_mac(char *mac_str, size_t len);
esp_netif_t* ethernet_w5500_get_netif(void);
```

##### Control
```c
esp_err_t ethernet_w5500_stop(void);
esp_err_t ethernet_w5500_restart(void);
```

#### Hardware Configuration

**Default GPIO Pins (NerdQAxePlus)**:
- MOSI: GPIO 12
- MISO: GPIO 16
- SCLK: GPIO 2
- CS: GPIO 21
- INT: Not connected (-1, polling mode)

**SPI Settings**:
- Host: SPI2_HOST
- Clock: 2 MHz (configurable 1-40 MHz)
- Mode: 0 (CPOL=0, CPHA=0)
- DMA: Auto-selected channel

#### MAC Address Generation

```c
// Locally administered MAC derived from ESP32 chip ID
uint8_t base_mac[6];
esp_efuse_mac_get_default(base_mac);

// Set locally administered bit and flip bit for differentiation
base_mac[0] |= 0x02;  // Locally administered
base_mac[0] ^= 0x01;  // Different from WiFi MAC
```

### 2.2 Configuration (Kconfig.projbuild)

#### Master Enable
```kconfig
config ENABLE_ETHERNET
    bool "Enable W5500 Ethernet Support"
    default n
```

#### GPIO Configuration
```kconfig
config W5500_SPI_MOSI
    int "SPI MOSI GPIO"
    default 12

config W5500_SPI_MISO
    int "SPI MISO GPIO"
    default 16

config W5500_SPI_SCLK
    int "SPI Clock GPIO"
    default 2

config W5500_SPI_CS
    int "SPI CS GPIO"
    default 21

config W5500_INT_GPIO
    int "Interrupt GPIO (-1 for polling)"
    default -1
```

#### Network Configuration
```kconfig
config W5500_USE_DHCP
    bool "Use DHCP"
    default y

config W5500_STATIC_IP
    string "Static IP Address"
    default "192.168.1.121"
    depends on !W5500_USE_DHCP
```

---

## 3. Core File Modifications

### 3.1 `main/main.cpp`

#### New Network Setup Function

**Location**: Lines 84-199
**Purpose**: Unified network initialization with Ethernet priority

```cpp
static void setup_network()
{
    // Initialize network infrastructure ONCE (critical!)
    network_infrastructure_init();

#ifdef CONFIG_ENABLE_ETHERNET
    char *network_mode_str = Config::getNetworkMode();
    bool use_ethernet = (strcmp(network_mode_str, "ethernet") == 0);

    if (use_ethernet) {
        ESP_LOGI(TAG, "Network mode: Ethernet - Initializing...");
        SYSTEM_MODULE.setNetworkMode(System::NETWORK_MODE_ETHERNET);

        // Initialize W5500 hardware
        ethernet_init_for_nerdaxe();
        SYSTEM_MODULE.setEthAvailable(ethernet_is_available());

        if (SYSTEM_MODULE.isEthernetAvailable()) {
            // Wait up to 10 seconds for DHCP IP
            char ip_buf[20];
            int retry_count = 0;
            while (retry_count < 100) {
                SYSTEM_MODULE.updateEthernetStatus();
                if (ethernet_get_ip(ip_buf, sizeof(ip_buf))) {
                    ESP_LOGI(TAG, "Ethernet connected: %s", ip_buf);
                    start_rest_server(NULL);
                    return;  // Success!
                }
                vTaskDelay(pdMS_TO_TICKS(100));
                retry_count++;
            }
            ESP_LOGW(TAG, "Ethernet timeout, falling back to WiFi");
        } else {
            ESP_LOGW(TAG, "W5500 not detected, falling back to WiFi");
        }

        // Explicit WiFi fallback
        SYSTEM_MODULE.setNetworkMode(System::NETWORK_MODE_WIFI);
    }
    free(network_mode_str);
#endif

    // WiFi mode (default or fallback)
    ESP_LOGI(TAG, "Network mode: WiFi");
    // ... WiFi initialization ...
}
```

**Key Changes**:
1. Single `network_infrastructure_init()` call before any interface
2. 10-second DHCP timeout with 100ms polling
3. Explicit fallback to WiFi on failure
4. Clear logging at each stage

### 3.2 `main/system.cpp`

#### New Member Variables

```cpp
class System {
private:
    NetworkMode m_networkMode;         // NETWORK_MODE_WIFI or NETWORK_MODE_ETHERNET
    bool m_ethAvailable;               // W5500 hardware detected
    bool m_ethLinkUp;                  // PHY link status
    bool m_ethConnected;               // Has IP address
    char m_ethIpAddress[IP4ADDR_STRLEN_MAX];
    char m_ethMacAddress[18];
};
```

#### Ethernet Status Update

```cpp
void System::updateEthernetStatus() {
#ifdef CONFIG_ENABLE_ETHERNET
    if (!m_ethAvailable || m_networkMode != NETWORK_MODE_ETHERNET) {
        return;
    }

    esp_netif_t* eth_netif = ethernet_w5500_get_netif();
    esp_netif_ip_info_t ip_info;

    if (esp_netif_get_ip_info(eth_netif, &ip_info) == ESP_OK &&
        ip_info.ip.addr != 0) {
        m_ethConnected = true;
        snprintf(m_ethIpAddress, sizeof(m_ethIpAddress),
                 IPSTR, IP2STR(&ip_info.ip));

        // Update system IP
        if (m_networkMode == NETWORK_MODE_ETHERNET) {
            strncpy(m_ipAddress, m_ethIpAddress, sizeof(m_ipAddress));
        }
    } else {
        m_ethConnected = false;
        m_ethLinkUp = ethernet_w5500_get_link_status();
        strcpy(m_ethIpAddress, "0.0.0.0");
    }
#endif
}
```

**Polling**: Called every 5 seconds in system task loop (line 446)

#### Display Update Logic

```cpp
void System::updateConnection() {
    if (m_networkMode == NETWORK_MODE_ETHERNET) {
#ifdef CONFIG_ENABLE_ETHERNET
        m_display->updateWifiStatus(m_ethConnected ? "LAN" : "LAN OFF");
#endif
    } else {
        m_display->updateWifiStatus(m_wifiStatus);
    }
}
```

### 3.3 `components/connect/connect.c`

#### Network Infrastructure Initialization

```c
void network_infrastructure_init(void)
{
    ESP_LOGI(TAG, "Initializing network infrastructure");
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    ESP_LOGI(TAG, "Network infrastructure initialized");
}
```

**Critical**: Must be called exactly ONCE before any WiFi or Ethernet initialization.

#### Ethernet Wrapper Functions

```c
#ifdef CONFIG_ENABLE_ETHERNET
void ethernet_init_for_nerdaxe(void) {
    esp_err_t ret = ethernet_w5500_init();
    if (ret == ESP_OK) {
        // Set hostname on Ethernet interface
        esp_netif_t *eth_netif = ethernet_w5500_get_netif();
        char *hostname = nvs_config_get_string(NVS_CONFIG_HOSTNAME, "NerdQAxe");
        esp_netif_set_hostname(eth_netif, hostname);
        free(hostname);
    }
}

bool ethernet_is_available(void) {
    return ethernet_w5500_is_available();
}

bool ethernet_get_ip(char* buf, size_t len) {
    return (ethernet_w5500_get_ip(buf, len) == ESP_OK);
}
// ... more wrappers
#endif
```

#### WiFi AP Security Fix

```c
// In IP_EVENT_STA_GOT_IP handler (line 118)
} else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
    // ... existing code ...

    // NEW: Disable WiFi AP mode after successful connection
    wifi_softap_off();
    ESP_LOGI(TAG, "WiFi AP mode disabled after successful connection");
}
```

---

## 4. Network Initialization Flow

### 4.1 Boot Sequence

```
1. app_main() starts
2. setup_network() called
   ├─ network_infrastructure_init()  ← CRITICAL: Called ONCE
   │   ├─ esp_netif_init()
   │   └─ esp_event_loop_create_default()
   │
   ├─ IF network_mode == "ethernet":
   │   ├─ ethernet_init_for_nerdaxe()
   │   │   ├─ SPI bus init
   │   │   ├─ 100ms stabilization delay
   │   │   ├─ W5500 MAC/PHY init
   │   │   ├─ Event handlers registered
   │   │   └─ esp_eth_start()
   │   │
   │   ├─ Wait for DHCP (max 10 seconds)
   │   ├─ IF success: START HTTP server, RETURN
   │   └─ IF fail: FALLBACK to WiFi
   │
   └─ wifi_init()  ← Default or fallback
       ├─ WiFi event handlers registered
       ├─ WiFi STA mode started
       ├─ WiFi AP mode started (fallback)
       └─ START HTTP server
3. Continue boot (tasks, mining, etc.)
```

### 4.2 Event-Driven State Management

#### Ethernet Events

```c
// ETH_EVENT_START → PHY initialization
// ETH_EVENT_CONNECTED → Link up (cable connected)
// ETH_EVENT_DISCONNECTED → Link down (cable unplugged)
// IP_EVENT_ETH_GOT_IP → DHCP assigned IP
```

#### WiFi Events

```c
// WIFI_EVENT_STA_START → STA mode started
// WIFI_EVENT_STA_CONNECTED → Connected to AP
// WIFI_EVENT_STA_DISCONNECTED → Connection lost
// IP_EVENT_STA_GOT_IP → DHCP assigned IP
```

---

## 5. API Changes

### 5.1 New C Functions (connect.c/connect.h)

```c
// Network infrastructure
void network_infrastructure_init(void);

// Ethernet wrappers
#ifdef CONFIG_ENABLE_ETHERNET
void ethernet_init_for_nerdaxe(void);
bool ethernet_is_connected(void);
bool ethernet_is_available(void);
bool ethernet_get_ip(char* buf, size_t len);
bool ethernet_get_mac(char* buf, size_t len);
#endif
```

### 5.2 New System Module Methods (system.h)

```cpp
// Network mode management
NetworkMode getNetworkMode() const;
void setNetworkMode(NetworkMode mode);

// Ethernet status
bool isEthernetAvailable() const;
void setEthAvailable(bool available);
bool isEthernetLinkUp() const;
bool isEthernetConnected() const;
const char* getEthernetIP() const;
const char* getEthernetMAC() const;
void updateEthernetStatus();
```

### 5.3 HTTP API Extensions

#### GET /api/system/info (Extended)

**New JSON Fields**:
```json
{
  "networkMode": "wifi" | "ethernet",
  "ethAvailable": 0 | 1,
  "ethLinkUp": 0 | 1,
  "ethConnected": 0 | 1,
  "ethIPv4": "192.168.1.121",
  "ethMac": "02:00:00:12:34:56"
}
```

#### GET /api/system/ethernet/status (New)

```json
{
  "networkMode": "ethernet",
  "ethAvailable": 1,
  "ethLinkUp": 1,
  "ethConnected": 1,
  "ethIPv4": "192.168.1.121",
  "ethMac": "02:00:00:12:34:56",
  "dhcpEnabled": true
}
```

#### POST /api/system/ethernet/config (New)

**Request**:
```json
{
  "networkMode": "ethernet",
  "dhcpEnabled": false,
  "staticIP": "192.168.1.100",
  "gateway": "192.168.1.1",
  "subnet": "255.255.255.0",
  "dns": "8.8.8.8"
}
```

**Security**: Requires `X-TOTP` header for authentication.

---

## 6. Configuration Options

### 6.1 NVS Configuration Keys

| Key | Type | Default | Description |
|-----|------|---------|-------------|
| `network_mode` | string | "wifi" | Active network mode |
| `eth_dhcp` | bool | true | DHCP enabled |
| `eth_static_ip` | string | "192.168.1.121" | Static IP (if DHCP disabled) |
| `eth_gateway` | string | "192.168.1.1" | Gateway IP |
| `eth_subnet` | string | "255.255.255.0" | Subnet mask |
| `eth_dns` | string | "8.8.8.8" | DNS server |

### 6.2 Config Accessors (nvs_config.h)

```cpp
namespace Config {
    char* getNetworkMode();
    void setNetworkMode(const char* value);

    bool isEthUseDHCP();
    void setEthUseDHCP(bool value);

    char* getEthStaticIP();
    void setEthStaticIP(const char* value);
    // ... more accessors
}
```

---

## 7. Build System Changes

### 7.1 Component Dependencies

**main/CMakeLists.txt**:
```cmake
PRIV_REQUIRES
    "ethernet_w5500"  # NEW: W5500 Ethernet driver
    "esp_eth"
    "esp_netif"
```

**components/ethernet_w5500/CMakeLists.txt**:
```cmake
idf_component_register(
    SRCS "ethernet_w5500.c"
    INCLUDE_DIRS "include"
    PRIV_REQUIRES
        "driver"      # SPI driver
        "esp_eth"     # Ethernet framework
        "esp_netif"   # Network interface
        "esp_event"   # Event loop
)
```

### 7.2 Conditional Compilation

All Ethernet code guarded by:
```c
#ifdef CONFIG_ENABLE_ETHERNET
    // Ethernet-specific code
#endif
```

**WiFi-only builds**: Set `CONFIG_ENABLE_ETHERNET=n` → Zero Ethernet code included

---

## 8. Testing & Validation

### 8.1 Test Scenarios

| Scenario | Expected Behavior |
|----------|------------------|
| **Ethernet Primary (Cable Connected)** | W5500 detected → DHCP IP within 10s → Display "LAN" |
| **Ethernet Primary (Cable Unplugged)** | W5500 detected → DHCP timeout → WiFi fallback |
| **Ethernet Primary (No Hardware)** | W5500 not detected → Immediate WiFi fallback |
| **WiFi Mode** | WiFi initialization only → No SPI traffic → Normal operation |
| **Network Mode Switch** | Config change → Reboot → New mode active |

### 8.2 Validation Checklist

- [x] W5500 hardware detection via SPI
- [x] MAC address generation (unique per device)
- [x] DHCP IP assignment
- [x] Static IP configuration
- [x] WiFi fallback mechanism
- [x] HTTP API responses
- [x] NVS configuration persistence
- [x] Display status update
- [x] Build with/without CONFIG_ENABLE_ETHERNET

---

## 9. Migration Guide

### 9.1 For WiFi-Only Users

**No action required**. Changes are backward-compatible:
- Default network mode remains WiFi
- Ethernet code conditionally compiled
- No functional impact on WiFi-only builds

### 9.2 For Ethernet Adopters

#### Hardware Requirements
- W5500 Ethernet module
- SPI wiring (MOSI=12, MISO=16, SCLK=2, CS=21)
- Ethernet cable + DHCP server or static IP config

#### Software Setup

1. **Enable Ethernet in menuconfig**:
   ```bash
   idf.py menuconfig
   → Ethernet W5500 Configuration
   → [*] Enable Ethernet W5500 Support
   ```

2. **Configure GPIO pins** (if different from defaults)

3. **Build and flash**:
   ```bash
   idf.py build flash monitor
   ```

4. **Set network mode via web UI**:
   - Navigate to Settings → Network
   - Select "Ethernet" mode
   - Configure DHCP or static IP
   - Save → Reboot

---

## 10. Troubleshooting

### 10.1 Common Issues

#### W5500 Not Detected

**Symptoms**: "W5500 hardware not detected"

**Solutions**:
1. Verify SPI wiring with multimeter
2. Check W5500 3.3V power supply
3. Review GPIO pin assignments in menuconfig
4. Enable ESP_LOG_DEBUG for detailed SPI logs

#### DHCP Timeout

**Symptoms**: "Ethernet timeout after 10 seconds, falling back to WiFi"

**Solutions**:
1. Verify DHCP server is running on network
2. Check Ethernet cable connection (link LED on W5500)
3. Configure static IP instead of DHCP
4. Check router DHCP pool availability

#### WiFi Fallback Not Working

**Symptoms**: System stuck, no WiFi connection

**Solutions**:
1. Verify WiFi credentials in NVS
2. Check `network_infrastructure_init()` called before interfaces
3. Review crash logs for SPI bus conflicts

### 10.2 Debug Logging

Enable verbose logging:
```c
esp_log_level_set("ethernet_w5500", ESP_LOG_DEBUG);
esp_log_level_set("esp_eth", ESP_LOG_DEBUG);
esp_log_level_set("esp_netif", ESP_LOG_DEBUG);
```

---

## Appendix A: File Summary

### New Files

| File | Lines | Purpose |
|------|-------|---------|
| `components/ethernet_w5500/ethernet_w5500.c` | 325 | Main W5500 driver implementation |
| `components/ethernet_w5500/include/ethernet_w5500.h` | 116 | Public API header |
| `components/ethernet_w5500/Kconfig.projbuild` | 78 | Build configuration options |
| `components/ethernet_w5500/CMakeLists.txt` | 8 | Component registration |

### Modified Files

| File | Changes | Description |
|------|---------|-------------|
| `main/main.cpp` | ~120 lines modified | Network initialization refactor |
| `main/system.cpp` | ~80 lines added | Ethernet status tracking |
| `main/system.h` | ~30 lines added | Ethernet state variables & methods |
| `components/connect/connect.c` | ~100 lines added | Ethernet wrappers, infrastructure init |
| `components/connect/include/connect.h` | ~10 lines added | Function declarations |
| `main/nvs_config.h` | ~20 lines added | Ethernet NVS keys |
| `main/http_server/handler_system.cpp` | ~50 lines added | API endpoints |

---

## Appendix B: References

- **W5500 Datasheet**: WIZnet W5500 Hardwired TCP/IP Ethernet Controller
- **ESP-IDF Ethernet API**: https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/network/esp_eth.html
- **ESP32-S3 TRM**: Espressif ESP32-S3 Technical Reference Manual
- **BitAxe Reference**: Dual network architecture inspiration

---

**Document Version**: 1.0
**Last Updated**: 2025-01-14
**Maintainer**: MLH BTC
**License**: Same as NerdQAxePlus firmware
