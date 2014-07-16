@setlocal
@cd /d "%~dp0"
@call "..\helper-scripts\help-uninstall-extension.cmd" --project-from-path %*
@endlocal
