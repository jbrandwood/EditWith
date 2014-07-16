// ****************************************************************************
// Run this script as "Administrator"

var WshShell = WScript.CreateObject( "WScript.Shell" );

MyDeleteKey(
	"HKCU\\SOFTWARE\\Classes\\*\\shellex\\ContextMenuHandlers\\UltraEdit\\" );

WScript.Quit( 0 );

// ****************************************************************************

function MyDeleteKey ( strKey )
{
	try { WshShell.RegDelete( strKey ); }
	catch ( err ) {}
}

// ****************************************************************************
