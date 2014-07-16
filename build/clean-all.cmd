@echo off

rem ***************************************************************************
rem ***************************************************************************
rem
rem clean.cmd
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

echo.
echo **************************************************************************
echo Deleting all shell extension build files ...
echo **************************************************************************
echo.

rem Clean up the Visual Studio projects.

if exist "..\vs2008\EditWith.ncb" del /f /q "..\vs2008\EditWith.ncb" >nul:
if errorlevel 1 goto :failed
if exist "..\vs2008\EditWith.suo" del /f /q "..\vs2008\EditWith.suo" >nul:
if errorlevel 1 goto :failed
del /f /q "..\vs2008\*.user" >nul: 2>&1 || ver >nul:

if exist "..\vs2010\EditWith.sdf" del /f /q "..\vs2010\EditWith.sdf" >nul:
if errorlevel 1 goto :failed
if exist "..\vs2010\EditWith.suo" del /f /q "..\vs2010\EditWith.suo" >nul:
if errorlevel 1 goto :failed
if exist "..\vs2010\ipch" rd /s /q "..\vs2010\ipch" >nul:
if errorlevel 1 goto :failed
del /f /q "..\vs2010\*.user" >nul: 2>&1 || ver >nul:

if "%SKIP_RESTART%"=="0" call "helper-scripts\help-restart-explorer.cmd"

rem Clean up .BAK files.

del /s /f /q "..\*.bak" >nul: 2>&1

rem Clean up the shell extension in every subdirectory.

for /f %%i in ('dir /ad /b') do (
 call :clean-extension "%%~i"
 if errorlevel 1 goto :failed
)

echo.
echo **************************************************************************
echo All shell extensions cleaned successfully
echo **************************************************************************
echo.

if "%PAUSE_AT_END%"=="1" pause
endlocal
exit /b 0

rem ***************************************************************************

:failed

echo.
echo **************************************************************************
echo Failed to clean all shell extensions (see mesages above for errors)
echo **************************************************************************
echo.

if "%PAUSE_AT_END%"=="1" pause
endlocal
exit /b 1

rem ***************************************************************************

rem Clean the shell extension in the directory that we've been passed.

:clean-extension
if exist "%~1\*.dll" del /f /q "%~1\*.dll" >nul:
if errorlevel 1 exit /b 1
del /f /q "%~1\*.exp" >nul: 2>&1
del /f /q "%~1\*.lib" >nul: 2>&1
del /f /q "%~1\*.pdb" >nul: 2>&1
if exist "%~1\Debug"   rd /s /q "%~1\Debug"   >nul:
if exist "%~1\Release" rd /s /q "%~1\Release" >nul:
exit /b 0

rem ***************************************************************************
