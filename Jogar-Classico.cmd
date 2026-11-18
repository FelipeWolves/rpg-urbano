@echo off
setlocal
set "RPG_EXECUTABLE=%~dp0build\combate\jogar.exe"
if not exist "%RPG_EXECUTABLE%" (
    echo O executavel ainda nao foi compilado nesta copia do projeto.
    pause
    exit /b 1
)
"%RPG_EXECUTABLE%" --classico
set "RPG_EXIT=%ERRORLEVEL%"
if not "%RPG_EXIT%"=="0" (
    echo A sessao encerrou com codigo %RPG_EXIT%.
    pause
)
exit /b %RPG_EXIT%
