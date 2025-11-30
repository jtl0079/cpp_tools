param(
    [string]$BuildDir = "$PWD/build",
    [string]$GeneratedRoot = "$BuildDir/generated_include",
    [string]$SourceRoot = "$PWD/include"
)

Get-ChildItem -Path $GeneratedRoot -Recurse -Include *.h,*.hpp | ForEach-Object {
    $rel = $_.FullName.Substring($GeneratedRoot.Length+1).TrimStart("\/")
    $dest = Join-Path $SourceRoot $rel
    $destDir = Split-Path $dest
    if(!(Test-Path $destDir)) { New-Item -ItemType Directory -Path $destDir -Force | Out-Null }
    Copy-Item -Path $_.FullName -Destination $dest -Force
    Write-Host "Copied $($_.FullName) -> $dest"
}
