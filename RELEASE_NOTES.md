# GitHub Release - V3.2 (TNA Edition)

## 📋 Release Information

**Tag Name:** `v3.2`
**Release Title:** `V3.2 (TNA Edition) - Dual Network Dual Pool Support`
**Target:** `main` branch

---

## 📝 Release Description (Copy to GitHub)

```markdown
# ESP-Miner NerdQAxePlus V3.2 - TNA Edition

## 🌟 Highlights

**Dual Network Dual Pool Architecture** - Production-ready firmware with native W5500 Ethernet support, intelligent WiFi fallback, and parallel pool mining capability.

## 🆕 What's New in V3.2

### Major Features
- ✅ **Dual Pool Mining** - Run two pool jobs in parallel for increased efficiency
- ✅ **True Dual Network** - WiFi or Ethernet (W5500) with runtime switching
- ✅ **Real-Time Monitoring** - Live Ethernet IP updates and PHY link detection
- ✅ **Network-Aware Stratum** - Intelligent connection management per network mode
- ✅ **Production Stability** - Critical bug fixes and reliability improvements

### Bug Fixes
- 🐛 Fixed OTP initialization NULL pointer crash on boot
- 🐛 Fixed Ethernet IP showing 0.0.0.0 in web UI
- 🐛 Fixed stratum "WiFi disconnected" spam in Ethernet mode
- 🐛 Fixed pool connection failures in Ethernet mode
- 🐛 Fixed version string display (now shows "TNA-V3.2")

### Performance Improvements
- ⚡ Optimized network reconnection logic (mode-aware)
- ⚡ Periodic Ethernet status updates (every 5 seconds)
- ⚡ Reduced unnecessary WiFi API calls in Ethernet mode
- ⚡ Actual W5500 PHY link status detection

### Technical Changes
- 🔧 NULL-safe hostname/MAC initialization
- 🔧 Correct boot order (network before board)
- 🔧 Separated WiFi/Ethernet code paths in stratum
- 🔧 Added `updateEthernetStatus()` periodic polling
- 🔧 ESP-IDF v5.3.4 native API usage

## 📦 What's Included

- Complete source code with all V3.2 enhancements
- W5500 Ethernet driver (ESP-IDF native)
- AxeOS web interface
- Build configuration and documentation
- CHANGELOG with detailed V3.1 → V3.2 changes

## 🔧 Requirements

- **Hardware**: ESP32-S3 (8MB Flash, 8MB PSRAM)
- **Platform**: ESP-IDF v5.3.4
- **Network**: WiFi or W5500 Ethernet module (optional)
- **Board**: NerdQAxePlus or compatible

## 🚀 Quick Start

### Clone & Build
```bash
git clone https://github.com/CryptoIceMLH/ESP-Miner-NerdQAxePlusLAN.git
cd ESP-Miner-NerdQAxePlusLAN
export BOARD=NERDQAXEPLUS2
idf.py build
idf.py -p /dev/ttyUSB0 flash monitor
```

### Enable Ethernet (Optional)
```bash
idf.py menuconfig
# Navigate to: Ethernet W5500 Configuration
# Enable: [*] Enable Ethernet W5500 Support
# Configure GPIO pins: MOSI=12, MISO=16, SCLK=2, CS=21
```

## 📖 Documentation

- **README.md** - Complete feature documentation
- **CHANGELOG.md** - V3.1 → V3.2 detailed changes
- **REPO_PREPARATION.md** - Developer setup guide

## 🙏 Support Development

https://www.molonlabe.holdings/

## ⚠️ Important Notes

- **Backward Compatible**: Drop-in replacement for V3.1
- **WiFi-Only Builds**: Ethernet can be disabled in menuconfig
- **No Binary Included**: Build from source (see Quick Start above)
- **Use at Own Risk**: No liability - see disclaimer in README

## 📝 Full Changelog

See [CHANGELOG.md](CHANGELOG.md) for complete list of changes from V3.1.

---

**Version:** TNA-V3.2
**Base Firmware:** V3.1
**Build Date:** December 2025
**ESP-IDF:** v5.3.4
**License:** Do as you want - not into licensing
```

---

## 🏷️ Git Tag Creation Commands

After you push the code, create and push the tag:

```bash
# Create annotated tag
git tag -a v3.2 -m "Release V3.2 (TNA Edition) - Dual Network Dual Pool Support"

# Push tag to GitHub
git push origin v3.2
```

---

## 📦 Optional: Build Firmware Binary for Release

If you want to include a pre-built firmware binary:

```bash
cd "e:\MLH BTC\ESP MINERS\ESP-Miner-NerdQAxePlus-V2-Lan\ESP-Miner-NerdQAxePlus-V3.2-Lan"
export BOARD=NERDQAXEPLUS2
idf.py build

# Binary will be in: build/esp-miner.bin
# You can attach this to the GitHub release
```

---

## 🎯 GitHub Release Checklist

- [ ] Push all code to main branch
- [ ] Create tag `v3.2`
- [ ] Create GitHub Release with tag `v3.2`
- [ ] Copy release description from above
- [ ] Attach firmware binary (optional)
- [ ] Publish release

---

**Ready for GitHub Release!**
