@echo off

rem ***************************************************************************
rem ***************************************************************************
rem
rem help-restart-explorer.cmd
rem
rem  Copyright John Brandwood 2014.
rem
rem  Distributed under the Boost Software License, Version 1.0.
rem  (See accompanying file LICENSE_1_0.txt or copy at
rem    http://www.boost.org/LICENSE_1_0.txt)
rem

echo.
echo **************************************************************************
echo Terminating Windows Explorer to unlock / reset all shell extensions
echo **************************************************************************
echo.

taskkill /f /im explorer.exe

echo.
echo **************************************************************************
echo Restarting Windows Explorer
echo **************************************************************************
echo.

start "" /i /d"%SystemRoot%" "%SystemRoot%\explorer.exe"

timeout 1 > nul:

rem ***************************************************************************
