# UUIDv7 installer — Windows (macOS/Linux: install.sh)
$ErrorActionPreference = 'Stop'

$base = 'https://github.com/Winterhart/uuidv7/releases/latest/download'
$asset = 'uuidv7-windows-x86_64.exe'
$dest = "$env:LOCALAPPDATA\Programs\uuidv7"

if ($env:PROCESSOR_ARCHITECTURE -ne 'AMD64') { throw "unsupported architecture: $($env:PROCESSOR_ARCHITECTURE)" }

$tmp = "$env:TEMP\uuidv7-install-$PID"
New-Item -ItemType Directory $tmp | Out-Null

Invoke-WebRequest "$base/$asset" -OutFile "$tmp\$asset"
Invoke-WebRequest "$base/SHA256SUMS" -OutFile "$tmp\SHA256SUMS"

$want = (Get-Content "$tmp\SHA256SUMS" | Where-Object { $_ -match $asset }).Split(' ')[0]
if (-not $want) { throw "no checksum for $asset in SHA256SUMS" }

$got = (Get-FileHash "$tmp\$asset" -Algorithm SHA256).Hash.ToLower()
if ($got -ne $want.ToLower()) { throw "checksum mismatch for $asset" }

New-Item -ItemType Directory $dest -Force | Out-Null
Copy-Item "$tmp\$asset" "$dest\uuidv7.exe" -Force
& "$dest\uuidv7.exe" | Out-Null
if ($LASTEXITCODE -ne 0) { throw 'binary failed to run' }

Remove-Item $tmp -Recurse -Force -ErrorAction SilentlyContinue

$path = [Environment]::GetEnvironmentVariable('Path', 'User')
if ($path -notlike "*$dest*") {
    [Environment]::SetEnvironmentVariable('Path', "$path;$dest", 'User')
    "install: added $dest to PATH — open a new terminal"
}
'install: done — run: uuidv7'
