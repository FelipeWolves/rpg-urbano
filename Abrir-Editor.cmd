@echo off
"D:\Unreal Engine\UE_5.8\Engine\Binaries\Win64\UnrealEditor.exe" "%~dp0experiments\unreal\RpgEngineProbe\RpgEngineProbe.uproject" -culture=en
if errorlevel 1 pause
