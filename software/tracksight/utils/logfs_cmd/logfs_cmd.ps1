# Attaches a USB drive / SD card reader to WSL2 with usbipd-win, then builds and runs logfs_cmd inside WSL.
# The drive is detached (handed back to Windows) when logfs_cmd exits.

$ErrorActionPreference = "Stop"

# usbipd bind needs admin, so relaunch elevated if we aren't already.
$isAdmin = ([Security.Principal.WindowsPrincipal][Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole(
    [Security.Principal.WindowsBuiltInRole]::Administrator)
if (-not $isAdmin) {
    Start-Process powershell -Verb RunAs -ArgumentList @(
        "-NoProfile", "-ExecutionPolicy", "Bypass", "-File", "`"$PSCommandPath`"")
    exit
}

function Find-Usbipd {
    $cmd = Get-Command usbipd -ErrorAction SilentlyContinue
    if ($cmd) { return $cmd.Source }
    $default = "$env:ProgramFiles\usbipd-win\usbipd.exe"
    if (Test-Path $default) { return $default }
    return $null
}

$busId = $null
try {
    # 1. Make sure usbipd-win is installed.
    $usbipd = Find-Usbipd
    if (-not $usbipd) {
        Write-Host "usbipd-win not found, installing with winget..." -ForegroundColor Yellow
        winget install --exact --id dorssel.usbipd-win --accept-source-agreements --accept-package-agreements
        $usbipd = Find-Usbipd
        if (-not $usbipd) { throw "usbipd-win install failed. Install it manually from https://github.com/dorssel/usbipd-win/releases" }
    }

    # usbipd-win 4.0 replaced `usbipd wsl attach` with `usbipd attach --wsl` and added `usbipd state`, which this
    # script uses, so upgrade older installs. `--version` prints e.g. "4.3.0+42.Branch.master.Sha.abc".
    $version = [string](& $usbipd --version 2>$null)
    $major = if ($version -match '^\s*(\d+)\.') { [int]$Matches[1] } else { 0 }
    if ($major -lt 4) {
        Write-Host "usbipd-win $($version.Trim()) is too old (need 4.0+), upgrading with winget..." -ForegroundColor Yellow
        winget upgrade --exact --id dorssel.usbipd-win --accept-source-agreements --accept-package-agreements
        $usbipd = Find-Usbipd
        $version = [string](& $usbipd --version 2>$null)
        if (-not ($version -match '^\s*(\d+)\.' -and [int]$Matches[1] -ge 4)) {
            throw "usbipd-win upgrade failed. Uninstall the old version, then install the latest from https://github.com/dorssel/usbipd-win/releases"
        }
    }

    # 2. Pick the USB device to pass through.
    $devices = @((& $usbipd state | ConvertFrom-Json).Devices | Where-Object { $_.BusId })
    if ($devices.Count -eq 0) { throw "No USB devices found. Plug in the SD card reader and try again." }

    $storage = @($devices | Where-Object { $_.Description -match "Mass Storage|Card Reader|Storage|SD" })
    if ($storage.Count -eq 1) {
        $busId = $storage[0].BusId
        Write-Host "Using $busId ($($storage[0].Description))"
    } else {
        Write-Host "USB devices:"
        $devices | ForEach-Object { Write-Host ("  {0,-8} {1}" -f $_.BusId, $_.Description) }
        $busId = (Read-Host "Enter the BUSID of the SD card reader").Trim()
        if (-not ($devices | Where-Object { $_.BusId -eq $busId })) { throw "Unknown BUSID '$busId'." }
    }

    # 3. Share it and attach it to WSL. WSL must be running for attach, so wake it first.
    & $usbipd bind --busid $busId
    wsl.exe -e true
    & $usbipd attach --wsl --busid $busId
    if ($LASTEXITCODE -ne 0) { throw "usbipd attach failed." }

    # 4. Build and run logfs_cmd inside WSL.
    $wslScript = (wsl.exe -e wslpath -a "$PSScriptRoot\logfs_cmd.sh").Trim()
    wsl.exe -e bash "$wslScript"
} catch {
    Write-Host "Error: $_" -ForegroundColor Red
} finally {
    if ($busId -and $usbipd) {
        Write-Host "Detaching $busId from WSL..."
        & $usbipd detach --busid $busId 2>$null
    }
    Read-Host "Press Enter to close"
}
