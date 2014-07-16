@setlocal
@cd /d "%~dp0"
@call "..\helper-scripts\help-install-extension.cmd" --project-from-path %*
@endlocal
