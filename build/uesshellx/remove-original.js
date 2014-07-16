// ****************************************************************************

var WshShell = WScript.CreateObject( "WScript.Shell" );

MyDeleteKey(
	"HKCU\\SOFTWARE\\Classes\\*\\shellex\\ContextMenuHandlers\\UEStudio\\" );

WScript.Quit( 0 );

// ****************************************************************************

function MyDeleteKey ( strKey )
{
	try { WshShell.RegDelete( strKey ); }
	catch ( err ) {}
}

// ****************************************************************************
