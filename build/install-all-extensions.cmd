@echo off

rem ***************************************************************************
rem ***************************************************************************
rem
rem install-all-extensions.cmd
rem
rem  Copyright John Brandwood 2014.
rem
rem  Distributed under the Boost Software License, Version 1.0.
rem  (See accompanying file LICENSE_1_0.txt or copy at
rem    http://www.boost.org/LICENSE_1_0.txt)
rem

setlocal

rem Simple command line parameter handling.

set PAUSE_AT_END=1
set SKIP_RESTART=0
:param1
if /i "%~1" neq "--no-pause" goto :param2
set PAUSE_AT_END=0
shift /1
:param2
if /i "%~1" neq "--no-restart-explorer" goto :param3
set SKIP_RESTART=1
shift /1
:param3

cd /d "%~dp0"

if "%SKIP_RESTART%"=="0" call "helper-scripts\help-restart-explorer.cmd"

rem Install the shell extensions in every subdirectory.

rem del /s *.dll

rem call clean-all-except-dlls.cmd

for /f %%i in ('dir /ad /b') do (
 call :install-extension "%%~i"
 if errorlevel 1 goto :failed
)

echo.
echo **************************************************************************
echo All shell extensions installed successfully
echo **************************************************************************
echo.

if "%PAUSE_AT_END%"=="1" pause
endlocal
exit /b 0

rem ***************************************************************************

:failed

echo.
echo **************************************************************************
echo Failed to install all shell extensions (see mesages above for errors)
echo **************************************************************************
echo.

if "%PAUSE_AT_END%"=="1" pause
endlocal
exit /b 1

rem ***************************************************************************

:install-extension
if not exist "%~1\install-extension.cmd" exit /b 0
call "%~1\install-extension.cmd" --no-pause --no-restart-explorer
goto :eof

rem ***************************************************************************
