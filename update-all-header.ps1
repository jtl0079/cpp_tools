Write-Host "====================================="
Write-Host " Updating ALL cpptools module headers"
Write-Host "====================================="


# 🔵 记录调用者运行 update-all-header.ps1 时所在的目录
$OriginalDir = Get-Location


# 当前脚本完整路径（一定要加这行！）
$currentScriptPath = $MyInvocation.MyCommand.Path

# 取得当前脚本所在目录
$Root = Split-Path -Parent $currentScriptPath

# 找到所有 update-*-header.ps1，但排除自己
$updateScripts = Get-ChildItem -Recurse -Path $Root -Filter "update-*-header.ps1" |
    Where-Object { $_.FullName -ne $currentScriptPath }

if ($updateScripts.Count -eq 0) {
    Write-Host "❌ No update scripts found." -ForegroundColor Red
    exit 1
}

# 按文件名排序后执行
foreach ($script in $updateScripts | Sort-Object FullName) {
    Write-Host "`n== Running: $($script.FullName)" -ForegroundColor Cyan

    & $script.FullName

    if ($LASTEXITCODE -ne 0) {
        Write-Host "❌ Update failed in $($script.FullName)" -ForegroundColor Red
        exit 1
    }
}

Write-Host "`n====================================="
Write-Host " All cpptools headers updated ✔"
Write-Host "=====================================" -ForegroundColor Green


# 🔵 回到执行脚本前的工作目录（你写代码的位置）
Set-Location $OriginalDir