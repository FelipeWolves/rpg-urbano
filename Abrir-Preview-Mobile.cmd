@echo off
"D:\Unreal Engine\UE_5.8\Engine\Binaries\Win64\UnrealEditor.exe" "%~dp0experiments\unreal\RpgEngineProbe\RpgEngineProbe.uproject" -game -culture=en -Presentation=Mobile -windowed -ResX=450 -ResY=800
if errorlevel 1 pause
