// ****************************************************************************

var WshShell = WScript.CreateObject( "WScript.Shell" );

MyDeleteKey(
	"HKCU\\SOFTWARE\\Classes\\*\\shellex\\ContextMenuHandlers\\TextPad\\" );

WScript.Quit( 0 );

// ****************************************************************************

function MyDeleteKey ( strKey )
{
	try { WshShell.RegDelete( strKey ); }
	catch ( err ) {}
}

// ****************************************************************************
