// ****************************************************************************

var WshShell = WScript.CreateObject( "WScript.Shell" );

MyWriteKey(
	"HKCU\\SOFTWARE\\Classes\\*\\shellex\\ContextMenuHandlers\\EditPlus 3\\",
	"{36D94110-787C-4828-9C1B-0DAFEBC36069}" );

WScript.Quit( 0 );

// ****************************************************************************

function MyWriteKey ( strKey, strValue )
{
	try { WshShell.RegWrite( strKey, strValue, "REG_SZ" ); }
	catch ( err ) {}
}

// ****************************************************************************
