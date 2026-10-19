@echo off
rem Commits everything not ignored (uses Saved\rd_commit_message.txt) and pushes to origin.
rem Uses Windows Git so Git LFS handles .uasset/.umap. Never forces, never rewrites history.
rem Log: Saved\Logs\rd\git.log  Result: Saved\Logs\rd\git.status
call "%~dp0rd_env.cmd"
cd /d "%RD_ROOT%"
echo RUNNING> "%RD_LOGS%\git.status"
set "MSG=%RD_ROOT%\Saved\rd_commit_message.txt"
(
  echo [%date% %time%] git sync
  "%RD_GIT%" lfs install --local
  "%RD_GIT%" add -A
  "%RD_GIT%" status --short
  if exist "%MSG%" ( "%RD_GIT%" commit -F "%MSG%" ) else ( echo No commit message file; skipping commit )
  "%RD_GIT%" push -u origin HEAD
) > "%RD_LOGS%\git.log" 2>&1
set "RC=%ERRORLEVEL%"
"%RD_GIT%" log --oneline -3 >> "%RD_LOGS%\git.log" 2>&1
> "%RD_LOGS%\git.status" echo EXIT=%RC%
echo Git sync finished with exit code %RC%. You can close this window.
timeout /t 5 >nul
exit /b %RC%
