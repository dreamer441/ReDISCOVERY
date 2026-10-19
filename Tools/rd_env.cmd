@echo off
rem Shared paths for REDISCOVERY helper scripts. Edit here if anything moves.
set "RD_ROOT=%~dp0.."
for %%I in ("%RD_ROOT%") do set "RD_ROOT=%%~fI"
set "RD_UPROJECT=%RD_ROOT%\REDISCOVERY.uproject"
set "RD_TARGET=RediscoveryEditor"
set "RD_ENGINE=D:\UE_5.8"
set "RD_GIT=D:\Git\bin\git.exe"
set "RD_LOGS=%RD_ROOT%\Saved\Logs\rd"
if not exist "%RD_LOGS%" mkdir "%RD_LOGS%"
