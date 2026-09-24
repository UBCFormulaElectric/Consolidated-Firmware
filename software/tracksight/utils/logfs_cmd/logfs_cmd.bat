@echo off
rem Double-click entry point for Windows: attaches an SD card / USB drive to WSL2 and launches logfs_cmd.
powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0logfs_cmd.ps1"
