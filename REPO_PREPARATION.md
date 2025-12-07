# GitHub Repository Update Guide - V3.2

## 📋 Pre-Push Checklist

### ✅ Files Ready
- [x] README.md - Updated with V3.2 features
- [x] CHANGELOG.md - V3.1 → V3.2 changes documented
- [x] CMakeLists.txt - PROJECT_VER set to "TNA-V3.2"
- [x] .gitignore - Configured to exclude build artifacts
- [x] LICENSE - Present
- [x] All source code files updated and tested

### ✅ Code Verification
- [x] W5500 GPIO pins correct (12/16/2/21)
- [x] OTP NULL pointer fix applied
- [x] Stratum Ethernet awareness implemented
- [x] System Ethernet IP monitoring added
- [x] Version string displays correctly
- [x] All V3.2 fixes compiled and tested

### ✅ Build Artifacts Cleaned
- [x] No `build/` directory
- [x] No `*.log` files in root
- [x] No `sdkconfig.old` files
- [x] Node modules handled by .gitignore

---

## 🚀 Pushing to GitHub

### Step 1: Initialize Git (if not already)

```bash
cd "e:\MLH BTC\ESP MINERS\ESP-Miner-NerdQAxePlus-V2-Lan\ESP-Miner-NerdQAxePlus-V3.2-Lan"

# Initialize git repository
git init

# Add remote (your existing repo)
git remote add origin https://github.com/CryptoIceMLH/ESP-Miner-NerdQAxePlusLAN.git
```

### Step 2: Stage All Files

```bash
# Add all files (respecting .gitignore)
git add .

# Verify what will be committed
git status
```

### Step 3: Create Commit

```bash
git commit -m "Release V3.2 (TNA Edition) - Dual Network Dual Pool Support

Major Features:
- Native W5500 Ethernet with dual network support
- Dual pool parallel mining capability
- Real-time Ethernet IP monitoring
- Actual PHY link status detection

Bug Fixes:
- Fixed OTP NULL pointer crash on boot
- Fixed Ethernet IP showing 0.0.0.0 in UI
- Fixed stratum WiFi spam in Ethernet mode
- Fixed pool connection issues in Ethernet mode

Performance:
- Optimized network reconnection logic
- Periodic Ethernet status updates
- Network-aware stratum management

Based on V3.1 with production-grade stability enhancements.
Version: TNA-V3.2 | Build: December 2025 | ESP-IDF v5.3.4"
```

### Step 4: Push to GitHub

**Option A: Replace Everything (Recommended for V3.2)**

```bash
# Fetch existing repo
git fetch origin main

# Force push V3.2 (replaces all old files)
git push -f origin main
```

**Option B: Create V3.2 Branch First**

```bash
# Create and switch to v3.2 branch
git checkout -b v3.2

# Push to new branch
git push -u origin v3.2

# Then merge to main via GitHub PR
```

---

## 📦 What Gets Pushed

### Included:
✅ All source code (.c, .cpp, .h files)
✅ Component directories with drivers
✅ Main application code
✅ Web UI source (axe-os)
✅ Configuration files (Kconfig, CMakeLists.txt)
✅ Documentation (README.md, CHANGELOG.md)
✅ Build scripts
✅ .github workflows
✅ LICENSE

### Excluded (by .gitignore):
❌ `build/` directory
❌ `sdkconfig` (user-specific)
❌ `sdkconfig.old`
❌ `.vscode/` settings (except versioned ones)
❌ `node_modules/` (too large, use npm install)
❌ `managed_components/` (use idf.py reconfigure)
❌ IDE-specific files (.idea, .DS_Store)

---

## 🔧 Post-Push User Instructions

After pushing, users should:

### 1. Clone the Repository
```bash
git clone https://github.com/CryptoIceMLH/ESP-Miner-NerdQAxePlusLAN.git
cd ESP-Miner-NerdQAxePlusLAN
```

### 2. Install Dependencies
```bash
# ESP-IDF components
idf.py reconfigure

# Web UI dependencies (optional, for rebuilding UI)
cd main/http_server/axe-os
npm install
cd ../../..
```

### 3. Configure Build
```bash
export BOARD=NERDQAXEPLUS2

# Optional: Enable Ethernet
idf.py menuconfig
# → Ethernet W5500 Configuration
# → [*] Enable Ethernet W5500 Support
```

### 4. Build and Flash
```bash
idf.py build
idf.py -p /dev/ttyUSB0 flash monitor
```

---

## 📝 GitHub Release Notes Template

Create a GitHub Release for V3.2:

**Tag:** `v3.2`
**Title:** `V3.2 (TNA Edition) - Dual Network Dual Pool Support`

**Description:**
```markdown
# ESP-Miner NerdQAxePlus V3.2 - TNA Edition

## 🌟 Highlights

- **Dual Network Support**: WiFi or Ethernet (W5500) with intelligent failover
- **Dual Pool Mining**: Run two pool jobs in parallel
- **Production Stability**: Fixed critical boot crashes and network issues
- **Real-Time Monitoring**: Accurate Ethernet IP and PHY link status

## 🆕 What's New in V3.2

### Features
- ✅ Dual pool parallel mining
- ✅ Real-time Ethernet IP monitoring
- ✅ W5500 PHY link status detection
- ✅ Network-aware stratum connections

### Bug Fixes
- 🐛 Fixed OTP initialization crash
- 🐛 Fixed Ethernet IP showing 0.0.0.0
- 🐛 Fixed stratum WiFi spam in Ethernet mode
- 🐛 Fixed version display

### Improvements
- ⚡ Optimized network reconnection logic
- ⚡ Periodic Ethernet status updates
- ⚡ Better NULL safety throughout

## 📥 Download

- **Firmware Binary**: Coming soon (or build from source)
- **Source Code**: See Assets below

## 🔧 Quick Start

See [README.md](README.md) for detailed setup instructions.

## 📝 Full Changelog

See [CHANGELOG.md](CHANGELOG.md) for complete V3.1 → V3.2 changes.

## ⚠️ Requirements

- ESP32-S3 (8MB Flash, 8MB PSRAM)
- ESP-IDF v5.3.4
- W5500 module (for Ethernet support)
- NerdQAxePlus hardware

## 🙏 Support Development

https://www.molonlabe.holdings/

---

**Based on NerdQAxePlus v3.1 firmware**
**License**: Do as you want - not into licensing
```

---

## ⚠️ Important Notes

1. **Backup First**: Make sure you have a backup of the current repo
2. **Test Build**: Verify firmware compiles before pushing
3. **Version Check**: Confirm version displays as "TNA-V3.2" when running
4. **Ethernet Test**: Verify Ethernet mode works if you have W5500 hardware
5. **Documentation**: Ensure README and CHANGELOG are accurate

---

## 🆘 Troubleshooting

### Issue: Too many files to push
**Solution**: Check .gitignore is working
```bash
git check-ignore -v main/http_server/axe-os/node_modules/
```

### Issue: Merge conflicts
**Solution**: Force push (V3.2 is meant to replace V3.1)
```bash
git push -f origin main
```

### Issue: Large file warnings
**Solution**: Exclude them in .gitignore
```bash
echo "path/to/large/file" >> .gitignore
git rm --cached path/to/large/file
```

---

**Ready to push!** Follow the steps above to update the GitHub repository.
