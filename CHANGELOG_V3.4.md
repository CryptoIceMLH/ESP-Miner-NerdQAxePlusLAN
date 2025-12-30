# CHANGELOG: V3.3BDOC → V3.4

## Version 3.4 - Current Protection Disable in BDOC Mode

---

## 🎯 Overview

V3.4 builds upon V3.3BDOC by removing current protection limits when BDOC Mode is enabled. This allows extreme overclocking without triggering "CURRENT PROTECTION" errors that previously limited power delivery during high-performance operations.

---

## ✨ New Features in V3.4

### 1. BDOC Current Protection Disable

**Problem Solved:**
- Previous versions had board-specific current limits (55A-95A depending on board type)
- When overclocking, miners would hit "CURRENT PROTECTION" guru meditation errors
- Limits were appropriate for normal operation but too restrictive for extreme overclocking

**Solution:**
- In BDOC Mode: TPS53647 voltage regulator current limit set to 1000A (effectively disabled)
- In Normal Mode: Standard board-specific limits remain (90A/95A/etc.)
- Applies to all board types: NERDQAXEPLUS, NERDQAXEPLUS2, NERDOCTAXEPLUS, NERDOCTAXEGAMMA, NERDQX

**Implementation Details:**
- Modified: [nerdqaxeplus.cpp](main/boards/nerdqaxeplus.cpp#L143-L150)
- Current limits programmed during `initAsics()` at boot time
- **IMPORTANT:** Toggling BDOC mode requires reboot for current limit changes to take effect
- Warning logged when BDOC current protection is disabled

**Code Changes:**
```cpp
// In BDOC mode, disable current protection by setting absurdly high limit
if (m_bdocMode) {
    ESP_LOGW(TAG, "BDOC MODE: Current protection DISABLED (set to 1000A)");
    m_tps->init(m_numPhases, 1000, 1000.0f);  // No board will ever hit 1000A
} else {
    m_tps->init(m_numPhases, m_imax, m_ifault);
}
```

---

## 🔄 Carried Forward from V3.3BDOC

All V3.3BDOC features remain unchanged:

### Three-Tier Support Level System
- **Level 0 (Safe Mode):** Dropdown menus for frequency/voltage
- **Level 1 (Advanced Mode):** Number inputs with validators
- **Level 2 (BDOC Mode):** All validators removed, BDOC features enabled

### BDOC Mode Features (Level 2 Only)
- Orange pulsing "BDOC FAFO MODE ENABLED" banner
- BDOC Overheat Temperature Control (40-120°C range)
- Immersion Mode Toggle (disable fan control)
- Removed frequency/voltage validators

---

## 🔒 Safety Features

1. **BDOC Mode Required:** Current protection only disabled when BDOC mode explicitly enabled
2. **Reboot Required:** Changes to BDOC mode require reboot (prevents accidental toggling)
3. **Visual Warnings:** Orange pulsing banner clearly indicates dangerous mode
4. **Board Agnostic:** 1000A limit works universally across all board types regardless of phase count
5. **Normal Mode Protection:** Standard current limits remain active when BDOC mode is disabled

---

## 📋 Migration Guide: V3.3BDOC → V3.4

### For End Users:
- **No action required** - firmware is fully backward compatible
- Existing V3.3BDOC behavior continues working
- New feature: Current protection automatically disabled when BDOC mode is enabled
- **REMEMBER:** Reboot after enabling/disabling BDOC mode for current limits to update

### Technical Changes:
- Modified: `main/boards/nerdqaxeplus.cpp` - Added conditional current limit logic
- No changes to TPS53647 driver code (kept simple)
- No UI/WebUI changes required

---

## 📦 Build Instructions

### Frontend (Angular):
```bash
cd main/http_server/axe-os
npm install          # Install dependencies
npm run build        # Build production bundle
```

**Output:** `dist/axe-os/*.js.gz` files with content-based hashes

### Firmware (ESP-IDF v5.3.4):
```bash
$env:BOARD = "NERDQAXEPLUS"   # or NERDQAXEPLUS2, NERDOCTAXEPLUS, NERDOCTAXEGAMMA, NERDQX
idf.py build
idf.py flash
```

**Supported Boards (all inherit BDOC current protection disable):**
- NERDQAXEPLUS
- NERDQAXEPLUS2
- NERDOCTAXEPLUS
- NERDOCTAXEGAMMA
- NERDQX

---

## ⚠️ WARNING

**BDOC Mode disables critical safety limits including:**
- Voltage/frequency validators (V3.3BDOC)
- Current protection (NEW in V3.4)
- Overheat protection (can be set up to 120°C)

**Risks include:**
- Hardware damage or destruction
- Fire hazard
- Voided warranties
- Potential injury

**Use BDOC Mode at your own risk. This firmware is intended for experienced overclockers who understand the risks.**

---

## 🐛 Known Issues

None reported.

---

## 📝 Version History

- **V3.4** - Current protection disable in BDOC mode
- **V3.3BDOC** - BDOC mode introduction, overheat temp control, immersion mode
- **V3.2** - Two-tier support level system
- **V3.1** - Initial LAN release

---
