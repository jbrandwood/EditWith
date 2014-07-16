// ****************************************************************************

var WshShell = WScript.CreateObject( "WScript.Shell" );

MyWriteKey(
	"HKCU\\SOFTWARE\\Classes\\*\\shellex\\ContextMenuHandlers\\UltraEdit\\",
	"{b5eedee0-c06e-11cf-8c56-444553540000}" );

WScript.Quit( 0 );

// ****************************************************************************

function MyWriteKey ( strKey, strValue )
{
	try { WshShell.RegWrite( strKey, strValue, "REG_SZ" ); }
	catch ( err ) {}
}

// ****************************************************************************
