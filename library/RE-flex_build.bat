@echo off

:: the script is looking for Visual Studio Developer Command Prompt
:: takes first one
:: then runs in the promp building of RE-flex library

if %PROCESSOR_ARCHITECTURE% == x86 (
  echo Host architecture is x86
  set cmd_file=vcvars32.bat
) else (
  echo Host architecture is x64
  set cmd_file=vcvars64.bat
)

echo Searching for Visual Studio Developer Command Prompt

for /f "delims=|" %%f in ('dir /b /s "\%cmd_file%"') do (
  set dev_cmd=%%f
  goto out
)
:out

echo Found in "%dev_cmd%"

set script_path=%~dp0
cd "%script_path%"RE-flex\vs

"%dev_cmd%" && clean.bat && build.bat && exit
