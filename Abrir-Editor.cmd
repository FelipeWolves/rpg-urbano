@echo off
powershell.exe -NoProfile -File "%~dp0scripts\abrir-preview.ps1" -Editor
if errorlevel 1 pause
