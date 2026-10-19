@echo off
rem Builds the editor target. Log: Saved\Logs\rd\build.log  Result: Saved\Logs\rd\build.status
call "%~dp0rd_env.cmd"
echo RUNNING> "%RD_LOGS%\build.status"
echo [%date% %time%] Build %RD_TARGET% > "%RD_LOGS%\build.log"
call "%RD_ENGINE%\Engine\Build\BatchFiles\Build.bat" %RD_TARGET% Win64 Development -Project="%RD_UPROJECT%" -WaitMutex -NoHotReloadFromIDE >> "%RD_LOGS%\build.log" 2>&1
set "RC=%ERRORLEVEL%"
> "%RD_LOGS%\build.status" echo EXIT=%RC%
echo Build finished with exit code %RC%. You can close this window.
timeout /t 5 >nul
exit /b %RC%
