# Pull Request: Add W5500 Ethernet/LAN Support

## Summary

This PR adds complete Ethernet/LAN capability to the NerdQAxePlus miner firmware using the WIZnet W5500 SPI Ethernet controller. The implementation provides dual network support (WiFi + Ethernet) with automatic fallback while maintaining full backward compatibility.

## Motivation

Many mining deployments prefer wired Ethernet connections for:
- **Reliability**: No WiFi interference or range issues
- **Performance**: Consistent latency and throughput
- **Security**: Physical network segmentation
- **Deployment**: Easier management in rack-mounted configurations

## Changes Overview

### New Components
- **`components/ethernet_w5500/`**: Complete W5500 SPI Ethernet driver (~450 lines)
  - ESP-IDF native driver implementation
  - Auto-generated unique MAC addresses
  - DHCP and static IP support
  - Event-driven state management

### Core Modifications
- **`main/main.cpp`**: Refactored network initialization with Ethernet priority
- **`main/system.cpp`**: Added Ethernet status tracking and display updates
- **`components/connect/`**: Network infrastructure abstraction and Ethernet wrappers
- **`main/http_server/`**: Extended API with Ethernet status and configuration endpoints

### Security Improvements
- **WiFi AP Auto-Disable**: Fixed security vulnerability where WiFi AP stayed active permanently
- **W5500 Stabilization**: Added 100ms hardware delay for reliable boot detection
- **TOTP Protection**: Ethernet configuration requires authentication

## Features

### User-Facing
✅ **Network Mode Selection**: Choose WiFi or Ethernet via web UI
✅ **Automatic Fallback**: Ethernet failures gracefully fall back to WiFi
✅ **Status Display**: Real-time connection status ("LAN" vs WiFi)
✅ **Flexible Configuration**: DHCP or static IP for Ethernet
✅ **Zero Breaking Changes**: WiFi-only users unaffected

### Technical
✅ **ESP-IDF 5.3+ Compatible**: Uses latest Ethernet framework APIs
✅ **Conditional Compilation**: Ethernet code optional (`CONFIG_ENABLE_ETHERNET`)
✅ **Professional Patterns**: Event-driven, memory-safe, comprehensive logging
✅ **Persistent Configuration**: NVS-based settings survive reboots

## Architecture

### Network Initialization Flow
```
Boot → network_infrastructure_init() (ONCE)
      ↓
  NVS: network_mode?
      ↓
   ┌──┴──┐
   │     │
Ethernet WiFi (default)
   │     │
   ├─ W5500 init
   ├─ Wait 10s for DHCP
   ├─ Success? → Done
   └─ Fail → WiFi fallback
```

### Hardware Configuration
- **SPI Pins**: MOSI=12, MISO=16, SCLK=2, CS=21
- **SPI Speed**: 2 MHz (configurable 1-40 MHz)
- **Operation**: Polling mode (no interrupt GPIO)
- **Power**: Standard 3.3V SPI

## API Changes

### HTTP Endpoints

#### Extended: `GET /api/system/info`
```json
{
  "networkMode": "wifi|ethernet",
  "ethAvailable": 0|1,
  "ethConnected": 0|1,
  "ethIPv4": "192.168.1.121",
  "ethMac": "02:00:00:12:34:56"
}
```

#### New: `GET /api/system/ethernet/status`
Returns detailed Ethernet configuration and link status.

#### New: `POST /api/system/ethernet/config`
Configure network mode and Ethernet settings (requires TOTP).

### System Module Methods
```cpp
NetworkMode getNetworkMode();
void setNetworkMode(NetworkMode mode);
bool isEthernetAvailable();
bool isEthernetConnected();
const char* getEthernetIP();
void updateEthernetStatus();
```

## Build Configuration

### Enable Ethernet
```bash
idf.py menuconfig
→ Ethernet W5500 Configuration
→ [*] Enable Ethernet W5500 Support
```

### Configure GPIO Pins
All pins configurable via menuconfig (defaults optimized for NerdQAxePlus).

### WiFi-Only Builds
Set `CONFIG_ENABLE_ETHERNET=n` → Zero Ethernet code included.

## Testing

### Validated Scenarios
- ✅ Ethernet mode with cable connected (DHCP)
- ✅ Ethernet mode with static IP
- ✅ Ethernet timeout → WiFi fallback
- ✅ W5500 hardware not detected → WiFi fallback
- ✅ WiFi mode (no Ethernet initialization)
- ✅ Network mode switching via web UI
- ✅ Build with/without Ethernet support

### Performance Impact
- **Boot time (Ethernet success)**: +100-500ms
- **Boot time (Ethernet timeout)**: +10 seconds
- **Boot time (WiFi mode)**: 0ms overhead
- **Runtime overhead**: ~5 kbps SPI traffic, negligible CPU

## Migration Guide

### For WiFi Users
**No action required.** All changes are backward-compatible.

### For Ethernet Users
1. Enable `CONFIG_ENABLE_ETHERNET` in menuconfig
2. Build and flash firmware
3. Connect W5500 module via SPI
4. Configure network mode in web UI → Reboot

### Rollback
Change network mode to "WiFi" in web UI or flash WiFi-only build.

## Documentation

### Comprehensive Technical Document
See [`ETHERNET_IMPLEMENTATION.md`](./ETHERNET_IMPLEMENTATION.md) for:
- Complete architecture documentation
- File-by-file code analysis
- API reference
- Configuration guide
- Troubleshooting procedures

### Updated README
Version 3.1 changelog added to [`readme.md`](./readme.md).

## Files Changed

### New Files (4)
- `components/ethernet_w5500/ethernet_w5500.c` (+325 lines)
- `components/ethernet_w5500/include/ethernet_w5500.h` (+116 lines)
- `components/ethernet_w5500/Kconfig.projbuild` (+78 lines)
- `components/ethernet_w5500/CMakeLists.txt` (+8 lines)

### Modified Files (8)
- `main/main.cpp` (~120 lines modified)
- `main/system.cpp` (~80 lines added)
- `main/system.h` (~30 lines added)
- `components/connect/connect.c` (~100 lines added)
- `components/connect/include/connect.h` (~10 lines added)
- `main/nvs_config.h` (~20 lines added)
- `main/http_server/handler_system.cpp` (~50 lines added)
- `CMakeLists.txt` (dependency added)

**Total**: ~800 lines added, ~100 lines modified

## Breaking Changes

**None.** All changes are additive and backward-compatible.

## Dependencies

### New ESP-IDF Components
- `esp_eth` - Ethernet driver framework (already in ESP-IDF)
- `driver` - SPI master driver (already in ESP-IDF)

### Hardware Requirements
- W5500 Ethernet module (optional)
- SPI wiring to ESP32-S3

## Known Limitations

1. **No Runtime Switching**: Changing network modes requires reboot
2. **Single Interface**: Cannot use WiFi + Ethernet simultaneously
3. **IPv4 Only**: No IPv6 support currently
4. **Polling Mode**: Interrupt GPIO not connected (acceptable performance)

## Future Enhancements

- [ ] Runtime network mode switching (no reboot)
- [ ] Link speed auto-detection (10/100 Mbps display)
- [ ] IPv6 dual-stack support
- [ ] Interrupt-driven operation for lower latency

## Credits

- **BitAxe Team**: Dual network architecture inspiration
- **WIZnet**: W5500 hardware and ioLibrary
- **Espressif**: ESP-IDF Ethernet framework

## Checklist

- [x] Code compiles without warnings
- [x] Tested on hardware (WiFi and Ethernet modes)
- [x] Documentation complete
- [x] Backward compatibility verified
- [x] Security review completed
- [x] API changes documented
- [x] Configuration options documented

## Reviewers

@shufps - Please review for merge consideration.

## License

Same as NerdQAxePlus firmware (check repository license).

---

**Version**: 3.1
**Author**: MLH BTC
**Date**: 2025-01-14
**ESP-IDF**: v5.3.x
