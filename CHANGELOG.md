# Changelog - V3.1 to V3.2

## V3.2 (TNA Edition) - December 2025

### 🆕 New Features

**Dual Pool Support:**
- ✅ Run two pool jobs in parallel for increased mining efficiency
- ✅ Automatic work distribution between primary and fallback pools
- ✅ Independent share tracking per pool

**Network Improvements:**
- ✅ Real-time Ethernet IP address monitoring and display
- ✅ Actual W5500 PHY link status detection (hardware-level)
- ✅ Network-aware stratum connection management
- ✅ Intelligent reconnection logic (network-mode aware)

**System Stability:**
- ✅ Version string now displays "TNA-V3.2" instead of generic "1"
- ✅ Fixed OTP initialization NULL pointer crash on boot
- ✅ NULL-safe hostname and MAC address handling
- ✅ Correct initialization order (network before board)

### 🔧 Bug Fixes

**Ethernet Mode:**
- 🐛 Fixed stratum "WiFi disconnected, attempting to reconnect..." spam in Ethernet mode
- 🐛 Fixed Ethernet IP showing 0.0.0.0 in web UI despite being connected
- 🐛 Fixed WiFi reconnection attempts when running in Ethernet mode
- 🐛 Fixed pool connection failures in Ethernet mode

**System:**
- 🐛 Fixed boot crash during OTP initialization (NULL pointer dereference)
- 🐛 Fixed system hanging when hostname/MAC unavailable at boot
- 🐛 Fixed version display showing incorrect value

### ⚡ Performance Optimizations

- Optimized WiFi reconnection logic to skip when in Ethernet mode
- Periodic Ethernet status updates (every 5 seconds) instead of one-time check
- Reduced unnecessary WiFi API calls in Ethernet mode
- Improved stratum connection reliability across both network modes

### 📝 Technical Changes

**Code Quality:**
- Added NULL safety checks for OTP hostname/MAC initialization
- Separated WiFi and Ethernet code paths in stratum task
- Added `isWifiConnected()` Ethernet awareness
- Implemented `updateEthernetStatus()` periodic polling in system task
- Added actual PHY link status from W5500 driver

**Build System:**
- Set explicit PROJECT_VER="TNA-V3.2" in CMakeLists.txt
- Maintained backward compatibility with WiFi-only builds

---

## V3.1 - January 2025

### Initial Release
- Native W5500 Ethernet support via SPI
- Dual network mode (WiFi or Ethernet)
- Automatic WiFi fallback on Ethernet failure
- Web UI network mode selection
- DHCP and static IP support for Ethernet
- Based on NerdQAxePlus v1.0.35+ firmware

---

**Migration from V3.1:**
- Drop-in replacement - no configuration changes required
- Existing Ethernet settings preserved
- Version automatically updates on flash
- All V3.1 features fully compatible

**Upgrade Benefits:**
- More stable Ethernet connections
- Better pool connectivity
- No more false WiFi warnings in Ethernet mode
- Accurate network status display
- Dual pool mining capability
