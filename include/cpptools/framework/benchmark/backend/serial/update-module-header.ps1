# ================================
# Update module aggregate header
# Trigger CMake reconfigure
# ================================

$ScriptRoot = Split-Path -Parent $MyInvocation.MyCommand.Path
$Module = Split-Path -Leaf $ScriptRoot
$BuildDir = Join-Path $ScriptRoot "build"

Write-Host "== Updating aggregate header for module: $Module ==" -ForegroundColor Cyan

# Save current directory
$OldDir = Get-Location

try {
    if (!(Test-Path $BuildDir)) {
        Write-Host "Build directory not found. Creating..." -ForegroundColor Yellow
        New-Item -ItemType Directory -Path $BuildDir | Out-Null
    }

    Set-Location $BuildDir

    Write-Host "Running CMake configure..." -ForegroundColor Green
    cmake ..

    Write-Host "Done ✅ $Module.h updated" -ForegroundColor Green
}
finally {
    # always restore original path
    Set-Location $OldDir
}
