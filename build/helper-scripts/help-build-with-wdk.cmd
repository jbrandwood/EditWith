@echo off

rem ***************************************************************************
rem ***************************************************************************
rem
rem help-build-with-wdk.cmd
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

echo.
echo **************************************************************************
echo **************************************************************************
echo Building "%SHELLX%" in "%SHELLX_DIR%" ...
echo **************************************************************************
echo **************************************************************************

echo.
echo **************************************************************************
echo Attempting to locate the Windows Driver Kit 7.1.0 in the registry.
echo **************************************************************************
echo.

call :find-ddk
if errorlevel 1 goto :build-failed
set ddkpath=%value%

echo Windows Driver Kit 7.1.0 found in %ddkpath%

if "%SKIP_RESTART%"=="0" call "..\helper-scripts\help-restart-explorer.cmd"

echo.
echo **************************************************************************
echo Removing old build files and directories.
echo **************************************************************************
echo.

if exist %SHELLX%32.dll ( del /f %SHELLX%32.* )
if errorlevel 1 ( goto :build-failed )
if exist %SHELLX%64.dll ( del /f %SHELLX%64.* )
if errorlevel 1 ( goto :build-failed )

if exist Debug ( rmdir /s /q Debug ) > nul:
if errorlevel 1 ( goto :build-failed )
if exist Release ( rmdir /s /q Release ) > nul:
if errorlevel 1 ( goto :build-failed )

cd ..\..\source\%SHELLX%

if exist buildfre_wxp_x86.log ( del /f buildfre_wxp_x86.* )
if exist buildfre_wnet_amd64.log ( del /f buildfre_wnet_amd64.* )

if exist objfre_wxp_x86 ( rmdir /s /q objfre_wxp_x86 ) > nul:
if errorlevel 1 ( goto :build-failed )
if exist objfre_wnet_amd64 ( rmdir /s /q objfre_wnet_amd64 ) > nul:
if errorlevel 1 ( goto :build-failed )

echo Old build files and directories removed successfully.

echo.
echo **************************************************************************
echo Building the 32-bit shell extension.
echo **************************************************************************
echo.

setlocal
pushd "%ddkpath%"
call "%ddkpath%\bin\setenv.bat" %ddkpath% fre x86 WXP no_oacr
if errorlevel 1 ( goto :build-failed )
popd
build.exe /c /x86
if errorlevel 1 ( goto :build-failed )
endlocal

echo.
echo 32-bit shell extension built successfully.

echo.
echo **************************************************************************
echo Building the 64-bit shell extension.
echo **************************************************************************
echo.

setlocal
pushd "%ddkpath%"
call "%ddkpath%\bin\setenv.bat" %ddkpath% fre x64 WNET no_oacr
if errorlevel 1 ( goto :build-failed )
popd
build.exe /c /amd64
if errorlevel 1 ( goto :build-failed )
endlocal

echo.
echo 64-bit shell extension built successfully.

echo.
echo **************************************************************************
echo Copying the shell extensions to "%SHELLX_DIR%" ...
echo **************************************************************************
echo.

copy /v objfre_wxp_x86\i386\%SHELLX%32.dll "%SHELLX_DIR%\%SHELLX%32.dll"
copy /v objfre_wnet_amd64\amd64\%SHELLX%64.dll "%SHELLX_DIR%\%SHELLX%64.dll"

del /f buildfre_wxp_x86.*
del /f buildfre_wnet_amd64.*

rem if exist objfre_wxp_x86 ( del /s /f /q objfre_wxp_x86 ) > nul:
rem if exist objfre_wnet_amd64 ( del /s /f /q objfre_wnet_amd64 ) > nul:

if exist objfre_wxp_x86 ( rmdir /s /q objfre_wxp_x86 ) > nul:
if exist objfre_wnet_amd64 ( rmdir /s /q objfre_wnet_amd64 ) > nul:

cd "%SHELLX_DIR%"

echo.
dir /n *.dll

if defined ProgramFiles(x86) (
 set extension=%SHELLX%64.dll
) else (
 set extension=%SHELLX%32.dll
)

regsvr32 /s "%extension%"
if errorlevel 1 ( goto :install-failed )

echo.
echo **************************************************************************
echo Successfully registered "%cd%\%extension%"
echo **************************************************************************

echo.
echo **************************************************************************
echo Build completed successfully
echo **************************************************************************
echo.

if "%PAUSE_AT_END%"=="1" pause
endlocal
exit /b 0

rem ***************************************************************************

:install-failed

echo.
echo **************************************************************************
echo Failed to register "%cd%\%extension%"
echo **************************************************************************
echo.

:build-failed

echo.
echo **************************************************************************
echo Build failed (see mesages above for errors)
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
echo Please run "build-with-wdk.cmd" in a shell extension's subdirectory.
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

:find-ddk

call :get-registry-key-value "HKLM\SOFTWARE\Microsoft\KitSetup\configured-kits\{B4285279-1846-49B4-B8FD-B9EAF0FF17DA}\{68656B6B-555E-5459-5E5D-6363635E5F61}" setup-install-location
if not errorlevel 1 ( exit /b 0 )
call :get-registry-key-value "HKLM\SOFTWARE\Wow6432Node\Microsoft\KitSetup\configured-kits\{B4285279-1846-49B4-B8FD-B9EAF0FF17DA}\{68656B6B-555E-5459-5E5D-6363635E5F61}" setup-install-location
if not errorlevel 1 ( exit /b 0 )
echo Unable to locate the Windows Driver Kit 7.1.0 in the registry. Aborting!
exit /b 1

rem ***************************************************************************

:get-registry-key-value

reg query "%~1" /v "%~2" > nul: 2>&1
if errorlevel 1 ( exit /b 1 )
for /f "skip=2 tokens=3*" %%i in ('reg query "%~1" /v "%~2"') do (
  set value=%%i%%j
  exit /b 0
)
exit /b 1

rem ***************************************************************************
