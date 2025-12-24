# CHANGELOG: V3.2 → V3.3BDOC

## Version 3.3BDOC - BDOC (Balls Deep Overclocking) Release

---

## 🎯 Overview

V3.3BDOC introduces advanced overclocking features for experienced users who want to push hardware beyond safe limits. This release maintains full V3.2 compatibility while adding a new "BDOC Mode" accessible only through deliberate user interaction (double-click).

---

## ✨ New Features

### 1. Three-Tier Support Level System

**Changed Behavior:**
- **Level 0 (Safe Mode):** Dropdown menus for frequency/voltage (unchanged from V3.2)
- **Level 1 (Advanced Mode):** Number inputs with validators (unchanged from V3.2)
- **Level 2 (BDOC/Pro Mode):** NEW - All validators removed, BDOC features enabled

**User Interface:**
- Warning triangle button tooltip changed from "Danger Zone" to "BDOC Zone (Balls Deep Overclocking)"
- Single-click: Toggle between Level 0 ↔ Level 1 (or Level 2 → Level 0)
- **Double-click: Enter Level 2 (BDOC Mode)**
- Button color changes:
  - Level 0: Basic (low opacity)
  - Level 1: Danger (red)
  - Level 2: Warning (orange/yellow) ← NEW

**Persistence:**
- Support level persists in browser localStorage
- Auto-expires after 24 hours (resets to Level 0)
- Single storage key: `support-level` (integer: 0, 1, or 2)

---

### 2. BDOC Mode Features (Level 2 Only)

When user double-clicks to reach Level 2, the following features become available:

#### A. Orange Pulsing Banner
- **Text:** "BDOC FAFO MODE ENABLED" (Find Out)
- **Styling:**
  - Orange background (`#f2a900`)
  - Red left border (`#ff3d71`)
  - Pulsing box-shadow animation (2s interval)
  - Warning triangle icon

#### B. BDOC Overheat Temperature Control
- **Purpose:** Allow higher temperature thresholds for extreme overclocking
- **Range:** 40-120°C (default: 90°C)
- **UI Location:** Visible ONLY at support level 2
- **Backend Integration:**
  - Separate NVS config key: `bdoc_overheat`
  - Power management uses BDOC temp when `bdocMode` is enabled
  - Normal mode continues using standard 70°C threshold


#### C. Immersion Mode Toggle
- **Purpose:** Disable fan control for liquid/dielectric cooling setups
- **UI Location:** Visible ONLY at support level 2
- **Behavior:**
  - When enabled: Fans report 0 RPM, all fan PWM output disabled
  - When disabled: Normal fan control restored
- **Backend Integration:**
  - NVS config key: `immersion_mode`
  - Bypasses fan control in `setFanSpeedCh()` and `getFanSpeedCh()`
  - No error warnings for stopped fans when enabled

#### D. Removed Validators
- **Frequency input:** No min/max limits (was: 400-800 MHz)
- **Voltage input:** No min/max limits (was: 1000-1400 mV)
- **WARNING:** Users can set ANY values - hardware damage possible!

---

### 3. Advanced Settings Visibility (Level 2)

The following advanced settings remain visible at Level 2 (carried over from V3.2 Level 1):
- Job Interval
- VR Frequency
- Stratum Difficulty

---


---

## 🔒 Safety Features

1. **24-Hour Expiry:** Support level automatically resets to Safe (0) after 1 day
2. **Double-Click Requirement:** BDOC mode requires deliberate user action (not accidental)
3. **Visual Warnings:** Orange pulsing banner clearly indicates dangerous mode
4. **No Auto-Enable:** BDOC features NEVER activate automatically
5. **Backward Compatible:** Users who never double-click never see BDOC features

---

## 📋 Migration Guide: V3.2 → V3.3BDOC

### For End Users:
- **No action required** - firmware is fully backward compatible
- Existing V3.2 behavior (2-tier system) continues working
- To access BDOC features: Double-click the warning triangle button



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
$env:BOARD = "NERDQAXEPLUS"   # or "NERDQAXEPLUS2"
idf.py build
idf.py flash
```

---


**WARNING:** BDOC Mode disables safety limits. Hardware damage, fire hazard, and voided warranties are possible. Use at your own risk.



