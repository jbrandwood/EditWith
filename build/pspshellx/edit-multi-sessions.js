// ****************************************************************************

var WshShell = WScript.CreateObject( "WScript.Shell" );

MyWriteKey(
	"HKCU\\SOFTWARE\\Classes\\CLSID\\{ED90173A-3B4C-4E7E-B9CF-79714425D4B5}\\",
	"EditWithPSPad" );
MyWriteKey(
	"HKCU\\SOFTWARE\\Classes\\CLSID\\{ED90173A-3B4C-4E7E-B9CF-79714425D4B5}\\AppOptions",
	"/N" );
MyWriteKey(
	"HKCU\\SOFTWARE\\Classes\\CLSID\\{ED90173A-3B4C-4E7E-B9CF-79714425D4B5}\\ShowExisting",
	"Y" );

WScript.Quit( 0 );

// ****************************************************************************

function MyWriteKey ( strKey, strValue )
{
	try { WshShell.RegWrite( strKey, strValue, "REG_SZ" ); }
	catch ( err ) {}
}

// ****************************************************************************
