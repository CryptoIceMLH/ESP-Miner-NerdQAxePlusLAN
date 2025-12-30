# V3.4 Repository Cleanup Script
# Run this from project root before committing to GitHub

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "  ESP-Miner V3.4 Repository Cleanup" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan

Write-Host "`nStarting repository cleanup..." -ForegroundColor Yellow

# Remove duplicate source folder
Write-Host "`n[1/4] Removing duplicate axe-os/axe-os folder..." -ForegroundColor Green
if (Test-Path "main\http_server\axe-os\axe-os")
{
    Remove-Item -Recurse -Force "main\http_server\axe-os\axe-os" -ErrorAction SilentlyContinue
    Write-Host "      Removed: main/http_server/axe-os/axe-os" -ForegroundColor Gray
}
else
{
    Write-Host "      Already clean (not found)" -ForegroundColor Gray
}

# Remove Angular build artifacts
Write-Host "`n[2/4] Removing Angular build artifacts..." -ForegroundColor Green

if (Test-Path "main\http_server\axe-os\.angular")
{
    Remove-Item -Recurse -Force "main\http_server\axe-os\.angular" -ErrorAction SilentlyContinue
    Write-Host "      Removed: .angular/" -ForegroundColor Gray
}

if (Test-Path "main\http_server\axe-os\dist")
{
    Remove-Item -Recurse -Force "main\http_server\axe-os\dist" -ErrorAction SilentlyContinue
    Write-Host "      Removed: dist/" -ForegroundColor Gray
}

if (Test-Path "main\http_server\axe-os\node_modules")
{
    Remove-Item -Recurse -Force "main\http_server\axe-os\node_modules" -ErrorAction SilentlyContinue
    Write-Host "      Removed: node_modules/" -ForegroundColor Gray
}

# Remove ESP-IDF build cache
Write-Host "`n[3/4] Removing ESP-IDF build cache..." -ForegroundColor Green

if (Test-Path "build")
{
    Remove-Item -Recurse -Force "build" -ErrorAction SilentlyContinue
    Write-Host "      Removed: build/" -ForegroundColor Gray
}

if (Test-Path "sdkconfig.old")
{
    Remove-Item -Force "sdkconfig.old" -ErrorAction SilentlyContinue
    Write-Host "      Removed: sdkconfig.old" -ForegroundColor Gray
}

# Verify cleanup
Write-Host "`n[4/4] Verifying cleanup..." -ForegroundColor Green
$remaining = @()
if (Test-Path "main\http_server\axe-os\axe-os") { $remaining += "main/http_server/axe-os/axe-os" }
if (Test-Path "main\http_server\axe-os\.angular") { $remaining += "main/http_server/axe-os/.angular" }
if (Test-Path "main\http_server\axe-os\dist") { $remaining += "main/http_server/axe-os/dist" }
if (Test-Path "main\http_server\axe-os\node_modules") { $remaining += "main/http_server/axe-os/node_modules" }
if (Test-Path "build") { $remaining += "build" }

Write-Host ""
if ($remaining.Count -eq 0)
{
    Write-Host "========================================" -ForegroundColor Green
    Write-Host "  Cleanup Successful! ✓" -ForegroundColor Green
    Write-Host "========================================" -ForegroundColor Green
    Write-Host "`nAll build artifacts and duplicate files removed." -ForegroundColor Cyan
}
else
{
    Write-Host "========================================" -ForegroundColor Red
    Write-Host "  Warning: Some files still exist" -ForegroundColor Red
    Write-Host "========================================" -ForegroundColor Red
    $remaining | ForEach-Object { Write-Host "  - $_" -ForegroundColor Red }
}

Write-Host "`n📋 Next Steps:" -ForegroundColor Yellow
Write-Host "  1. Review changes: git status" -ForegroundColor Gray
Write-Host "  2. Add files: git add ." -ForegroundColor Gray
Write-Host "  3. Commit: git commit -m 'feat: BDOC Current Protection Disable (V3.4)'" -ForegroundColor Gray
Write-Host "  4. Push to GitHub: git push origin develop" -ForegroundColor Gray
Write-Host ""
