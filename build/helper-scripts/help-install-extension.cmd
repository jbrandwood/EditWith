@echo off

rem ***************************************************************************
rem ***************************************************************************
rem
rem help-install-extension.cmd
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
if /i "%~1" neq "--project-from-path" goto :no-run-standalone
shift /1
:param2
if /i "%~1" neq "--no-pause" goto :param3
set PAUSE_AT_END=0
shift /1
:param3
if /i "%~1" neq "--no-restart-explorer" goto :param4
set SKIP_RESTART=1
shift /1
:param4

rem Set the extension name and location fron the current directory.

call :set-shellx-name "%cd%"

if defined ProgramFiles(x86) (
 set extension=%SHELLX%64.dll
) else (
 set extension=%SHELLX%32.dll
)

if not exist "%extension%" ( goto :not-found )

regsvr32 /s "%extension%"
if errorlevel 1 ( goto :reg-error )

:succeeded

echo.
echo **************************************************************************
echo Successfully registered "%extension%"
echo **************************************************************************
echo.

if "%SKIP_RESTART%"=="0" call "..\helper-scripts\help-restart-explorer.cmd"

echo **************************************************************************
echo Installation completed successfully
echo **************************************************************************
echo.

if "%PAUSE_AT_END%"=="1" pause
endlocal
exit /b 0

:not-found

echo.
echo **************************************************************************
echo Cannot find "%extension%" in "%cd%"
echo **************************************************************************
goto :failed

:reg-error

echo.
echo **************************************************************************
echo Cannot register "%extension%"
echo **************************************************************************
goto :failed

:failed

echo.
echo **************************************************************************
echo Installation failed (see mesage above for the error)
echo **************************************************************************
echo.

if "%PAUSE_AT_END%"=="1" pause
endlocal
exit /b 1

rem ***************************************************************************

:no-run-standalone

echo.
echo **************************************************************************
echo Please do not call "%~nx0".
echo Please run "install-extension.cmd" in a shell extension's subdirectory.
echo **************************************************************************
echo.

pause
exit /b 1

rem ***************************************************************************

:set-shellx-name

set SHELLX=%~n1
set SHELLX_DIR=%~1
exit /b 0

rem ***************************************************************************
