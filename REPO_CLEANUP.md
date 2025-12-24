# Repository Cleanup Guide - V3.3BDOC

## Files/Folders to DELETE Before Committing

Run these commands from the project root:

### 1. Remove Duplicate Source Folder
```bash
rm -rf "main/http_server/axe-os/axe-os"
```
**Why:** This was a duplicate copy of the Angular source code. The actual source is in `main/http_server/axe-os/src/`.

### 2. Remove Build Artifacts
```bash
cd main/http_server/axe-os
rm -rf .angular
rm -rf dist
rm -rf node_modules
```
**Why:** These are generated during build and should not be in version control.

### 3. Remove ESP-IDF Build Cache
```bash
cd ../../../
rm -rf build
rm -rf sdkconfig.old
```
**Why:** Build output and old SDK configs - regenerated during `idf.py build`.

---

## .gitignore Additions

Add these entries to `main/http_server/axe-os/.gitignore`:

```gitignore
# Angular build cache
.angular/
dist/

# Dependencies
node_modules/

# OS files
.DS_Store
Thumbs.db

# IDE
.vscode/
.idea/
*.swp
*.swo

# Logs
npm-debug.log*
yarn-debug.log*
yarn-error.log*
```

Add these to root `.gitignore`:

```gitignore
# ESP-IDF build
build/
sdkconfig.old

# Partition binaries
*.bin

# Python cache
__pycache__/
*.pyc
```

---

## Verification Commands

After cleanup, verify clean state:

```bash
# Should show NO node_modules, dist, .angular, or axe-os/axe-os
find . -type d -name "node_modules" -o -name "dist" -o -name ".angular"

# Should show NO build folder
ls -la | grep build

# Git status should only show actual source files
git status
```

---

## Clean Build Test

After cleanup, test that everything still builds:

### 1. Frontend
```bash
cd main/http_server/axe-os
npm install       # Reinstall dependencies
npm run build     # Should succeed
```

### 2. Firmware
```bash
cd ../../../
$env:BOARD = "NERDQAXEPLUS"
idf.py build      # Should succeed
```

---

## Repository Structure (After Cleanup)

```
ESP-Miner-NerdQAxePlus-V3.3BDOC/
├── main/
│   ├── http_server/
│   │   └── axe-os/
│   │       ├── src/           ← Angular source (KEEP)
│   │       ├── angular.json   ← Angular config (KEEP)
│   │       ├── package.json   ← NPM config (KEEP)
│   │       ├── .gitignore     ← Git ignore rules (KEEP)
│   │       ├── .angular/      ← DELETE (build cache)
│   │       ├── dist/          ← DELETE (build output)
│   │       ├── node_modules/  ← DELETE (dependencies)
│   │       └── axe-os/        ← DELETE (duplicate source)
│   ├── nvs_config.h
│   ├── tasks/
│   ├── boards/
│   └── ...
├── components/
├── build/                     ← DELETE (ESP-IDF output)
├── sdkconfig
├── sdkconfig.old              ← DELETE (old config)
├── CMakeLists.txt
├── CHANGELOG_V3.3BDOC.md      ← KEEP (this file)
├── REPO_CLEANUP.md            ← KEEP (this file)
└── README.md
```

---

## PowerShell Cleanup Script

Run this from project root:

```powershell
# V3.3BDOC Repository Cleanup Script

Write-Host "Starting repository cleanup..." -ForegroundColor Yellow

# Remove duplicate source folder
Write-Host "`n[1/4] Removing duplicate axe-os/axe-os folder..."
Remove-Item -Recurse -Force "main\http_server\axe-os\axe-os" -ErrorAction SilentlyContinue

# Remove Angular build artifacts
Write-Host "`n[2/4] Removing Angular build artifacts..."
Remove-Item -Recurse -Force "main\http_server\axe-os\.angular" -ErrorAction SilentlyContinue
Remove-Item -Recurse -Force "main\http_server\axe-os\dist" -ErrorAction SilentlyContinue
Remove-Item -Recurse -Force "main\http_server\axe-os\node_modules" -ErrorAction SilentlyContinue

# Remove ESP-IDF build cache
Write-Host "`n[3/4] Removing ESP-IDF build cache..."
Remove-Item -Recurse -Force "build" -ErrorAction SilentlyContinue
Remove-Item -Force "sdkconfig.old" -ErrorAction SilentlyContinue

# Verify cleanup
Write-Host "`n[4/4] Verifying cleanup..."
$remaining = @()
if (Test-Path "main\http_server\axe-os\axe-os") { $remaining += "axe-os/axe-os" }
if (Test-Path "main\http_server\axe-os\.angular") { $remaining += ".angular" }
if (Test-Path "main\http_server\axe-os\dist") { $remaining += "dist" }
if (Test-Path "main\http_server\axe-os\node_modules") { $remaining += "node_modules" }
if (Test-Path "build") { $remaining += "build" }

if ($remaining.Count -eq 0) {
    Write-Host "`nCleanup successful! ✓" -ForegroundColor Green
} else {
    Write-Host "`nWarning: Some files still exist:" -ForegroundColor Red
    $remaining | ForEach-Object { Write-Host "  - $_" -ForegroundColor Red }
}

Write-Host "`nRepository is ready for commit." -ForegroundColor Cyan
```

Save as `cleanup-repo.ps1` and run:
```powershell
.\cleanup-repo.ps1
```

---

## Git Commit Checklist

Before committing:

- [ ] Duplicate `axe-os/axe-os` folder removed
- [ ] `.angular`, `dist`, `node_modules` removed
- [ ] `build` folder removed
- [ ] `.gitignore` updated
- [ ] `CHANGELOG_V3.3BDOC.md` present
- [ ] Clean build test passed (frontend + firmware)
- [ ] Git status shows only source files

---

## Recommended Commit Message

```
feat: Add BDOC Mode (V3.3BDOC) - Extreme Overclocking Features

- Added 3-tier support level system (Safe/Advanced/BDOC)
- BDOC mode accessible via double-click warning button
- New features in BDOC mode:
  * Removed frequency/voltage validators
  * BDOC overheat temperature control (40-120°C)
  * Immersion mode toggle (disables fans)
  * Orange pulsing warning banner
- 24-hour support level expiry for safety
- Fully backward compatible with V3.2
- Frontend implementation complete
- Backend implementation pending (NVS config, power mgmt, fan control)

See CHANGELOG_V3.3BDOC.md for full details.

Closes #[issue-number]
```

---

## Post-Commit Actions

1. **Tag the release:**
   ```bash
   git tag -a v3.3BDOC -m "Release V3.3BDOC - BDOC Features"
   git push origin v3.3BDOC
   ```

2. **Create GitHub Release:**
   - Use content from `CHANGELOG_V3.3BDOC.md`
   - Attach compiled firmware binary (optional)

3. **Update main README.md:**
   - Add V3.3BDOC to version history
   - Document BDOC features
   - Add safety warnings

---

**Last Updated:** 2024-12-24
