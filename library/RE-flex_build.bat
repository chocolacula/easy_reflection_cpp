@echo off

:: the script is looking for Visual Studio Developer Command Prompt
:: takes first one
:: then runs in the promp building of RE-flex library

echo Searching for Visual Studio Developer Command Prompt

for /f "delims=|" %%f in ('dir /b /s "\VsDevCmd.bat"') do (
  set dev_cmd=%%f
  goto out
)
:out

echo Found in "%dev_cmd%"

set script_path=%~dp0
cd "%script_path%"RE-flex\vs

"%dev_cmd%" && build.bat && exit
