@setlocal
@cd /d "%~dp0"
@call "..\helper-scripts\help-build-with-wdk.cmd" --project-from-path %*
@endlocal
