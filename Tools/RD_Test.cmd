@echo off
rem Runs automation tests whose names start with "Rediscovery" headlessly.
rem Log: Saved\Logs\rd\test.log  Result: Saved\Logs\rd\test.status
call "%~dp0rd_env.cmd"
set "RD_FILTER=%~1"
if "%RD_FILTER%"=="" set "RD_FILTER=Rediscovery"
echo RUNNING> "%RD_LOGS%\test.status"
"%RD_ENGINE%\Engine\Binaries\Win64\UnrealEditor-Cmd.exe" "%RD_UPROJECT%" -ExecCmds="Automation RunTests %RD_FILTER%; Quit" -unattended -nopause -nullrhi -nosplash -NoSound -TestExit="Automation Test Queue Empty" -ReportExportPath="%RD_LOGS%\test-report" -abslog="%RD_LOGS%\test.log"
set "RC=%ERRORLEVEL%"
> "%RD_LOGS%\test.status" echo EXIT=%RC%
echo Tests finished with exit code %RC%. You can close this window.
timeout /t 5 >nul
exit /b %RC%
