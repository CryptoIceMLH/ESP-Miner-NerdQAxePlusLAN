# ESP-Miner NerdQAxePlus V3.4 - TNA Edition LAN and OVERCLOCKING READY

**Version:** TNA-V3.4
**Release Date:** December 2025
**Platform:** ESP32-S3 (ESP-IDF v5.3.4)
**Hardware:** NerdQAxePlus Bitcoin Mining Board

##### Support Development: <a href="https://www.molonlabe.holdings/" target="_blank">https://www.molonlabe.holdings/</a>

---

## 🚀 Overview

**ESP-Miner NerdQAxePlus (TNA Edition)** dual network dual pool support with native W5500 Ethernet, intelligent fallback, and production-grade stability enhancements.

### 🌟 What Makes V3.2 Special

#### 🔌 **TRUE Dual Network Duel Pool Architecture**
- **Ethernet Mode**: W5500 SPI Ethernet with hardware link detection
- **WiFi Mode**: Traditional wireless with AP fallback portal
- **Dual Pool mode**: Run two pool jobs in parallel
- **Intelligent Switching**: Runtime network mode selection via web UI
- **Automatic Failover**: Seamless WiFi fallback on Ethernet timeout/failure
- **Zero Downtime**: Hot-switchable between network modes with simple reboot


## 🔧 Hardware Requirements

### Minimum Requirements

- **ESP32-S3** microcontroller (8MB Flash, 8MB PSRAM)
- **ASIC Mining Chip**: BM1366, BM1368, or BM1370
- **Display**: ST7789 LCD (optional but recommended)
- **Power Supply**: 12V DC, capable of delivering peak current for ASIC
- **Network**: WiFi or Ethernet (W5500 module)

### Supported Boards

- **NerdQAxePlus** -
### Ethernet Hardware (Optional)

If using Ethernet mode:
- **Controller**: WIZnet W5500 SPI Ethernet module
- **Connection**: Cat5e/Cat6 Ethernet cable
- **Network**: DHCP-enabled router or static IP configuration

**Default GPIO Mapping for W5500:**
```
MOSI: GPIO 12
MISO: GPIO 16
SCLK: GPIO 2
CS:   GPIO 21
INT:  Not connected (polling mode)
```

---

## 🚀 Quick Start

### 1. First Boot

1. Power on the device
2. Device boots in **WiFi AP mode** with SSID: `NerdQAxePlus-XXXXXX`
3. Connect to the WiFi AP (no password required initially)
4. Navigate to `http://192.168.4.1` in your browser
5. Complete initial setup wizard

### 2. Network Configuration

**WiFi Mode (Default):**
1. Go to **Settings → Network**
2. Enter your WiFi SSID and password
3. Click **Save & Reboot**
4. Device connects to your WiFi network
5. Find device IP in your router's DHCP client list

**Ethernet Mode:**
1. Connect W5500 module to SPI pins (see GPIO mapping above)
2. Connect Ethernet cable to W5500 and router
3. In web UI: **Settings → Network → Mode**
4. Select **Ethernet**
5. Choose **DHCP** or configure static IP
6. Click **Save & Reboot**
7. Device reboots and acquires IP via Ethernet


---

## 🌐 Network Modes

### WiFi Mode

**Advantages:**
- No additional hardware required
- Easy setup
- Mobile/portable operation

**Configuration:**
- Supports WPA2/WPA3 security
- Automatic reconnection on disconnect
- AP fallback mode for recovery

**WiFi Status Indicators:**
- `Connected` - Active WiFi connection
- `Connecting...` - Attempting connection
- `AP Mode` - Fallback configuration portal

### Ethernet Mode

**Advantages:**
- More stable connection
- Lower latency
- No WiFi interference
- Better for permanent installations



---

## 🖥️ Web Interface

### Dashboard

The main dashboard displays real-time mining statistics:

- **Hashrate**: Current, 1-minute, 5-minute, 10-minute averages
- **Pool Status**: Connected pool, shares accepted/rejected
- **Temperature**: ASIC chip temperature
- **Power**: Voltage, current, wattage
- **Best Share**: Highest difficulty share found
- **Uptime**: System uptime
- **Network**: IP address, connection status

### Settings Pages

#### Network Settings
- Network mode selection (WiFi/Ethernet)
- WiFi credentials
- Ethernet configuration (DHCP/Static)
- Hostname

#### Pool Settings
- Primary pool configuration
- Fallback pool configuration
- Stratum difficulty
- Keepalive settings

#### ASIC Settings
- Frequency (MHz)
- Voltage (mV)
- ASIC model selection
- Temperature limits

#### System Settings
- Timezone
- Display brightness
- LED control
- OTA updates
- Factory reset

#### Security Settings
- TOTP (2FA) setup
- Admin password
- API access tokens

---

## ⛏️ Pool Configuration


Dual pool or pool failover available 

---

## 📊 Monitoring & Alerts

### InfluxDB Integration

Send metrics to InfluxDB for long-term monitoring:

**Configuration:**
```
Settings → Monitoring → InfluxDB
  URL: http://your-influxdb-server:8086
  Database: esp_miner
  Username: miner
  Password: your_password
  Interval: 60 seconds
```

**Metrics Collected:**
- Hashrate (current, averages)
- Shares (accepted, rejected, difficulty)
- Temperature
- Power consumption
- Uptime

### Discord Webhooks

Get notified of important events:

**Configuration:**
```
Settings → Alerts → Discord
  Webhook URL: https://discord.com/api/webhooks/...
```

**Alert Types:**
- Block found! 🎉
- Mining started
- Pool connection lost
- Temperature warning
- Low hashrate alert



## 📄 License

Do as you want - not into licensing 

---

## 🙏 Credits

**NerdQAxePlus Platform:**
- as per notes in V3.1

---



## ⚠️ Disclaimer

**This firmware is to be used at your own risk - I take zero liability.**


**Version:** TNA-V3.2
**Build Date:** December 2025
**ESP-IDF:** v5.3.4
**Target:** ESP32-S3
