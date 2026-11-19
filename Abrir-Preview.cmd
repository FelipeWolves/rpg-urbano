@echo off
powershell.exe -NoProfile -File "%~dp0scripts\abrir-preview.ps1"
if errorlevel 1 pause
