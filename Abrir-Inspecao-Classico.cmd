@echo off
"D:\Unreal Engine\UE_5.8\Engine\Binaries\Win64\UnrealEditor.exe" "%~dp0experiments\unreal\RpgEngineProbe\RpgEngineProbe.uproject" -game -Arena -Inspection -ArenaLayout=Classic -CombatProfile=Classic -culture=en -windowed -ResX=1280 -ResY=720
if errorlevel 1 pause
