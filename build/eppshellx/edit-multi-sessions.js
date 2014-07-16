// ****************************************************************************

var WshShell = WScript.CreateObject( "WScript.Shell" );

MyWriteKey(
	"HKCU\\SOFTWARE\\Classes\\CLSID\\{E8D5FF73-D41E-41CD-9779-E3B05DC5BA1F}\\",
	"EditWithEditPad" );
MyWriteKey(
	"HKCU\\SOFTWARE\\Classes\\CLSID\\{E8D5FF73-D41E-41CD-9779-E3B05DC5BA1F}\\AppOptions",
	"/newinstance" );
MyWriteKey(
	"HKCU\\SOFTWARE\\Classes\\CLSID\\{E8D5FF73-D41E-41CD-9779-E3B05DC5BA1F}\\ShowExisting",
	"Y" );

WScript.Quit( 0 );

// ****************************************************************************

function MyWriteKey ( strKey, strValue )
{
	try { WshShell.RegWrite( strKey, strValue, "REG_SZ" ); }
	catch ( err ) {}
}

// ****************************************************************************
