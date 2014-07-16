// ****************************************************************************

var WshShell = WScript.CreateObject( "WScript.Shell" );

MyWriteKey(
	"HKCU\\SOFTWARE\\Classes\\CLSID\\{7B245014-8540-4190-936A-D0B89838CC40}\\",
	"EditWithEmEditor" );
MyWriteKey(
	"HKCU\\SOFTWARE\\Classes\\CLSID\\{7B245014-8540-4190-936A-D0B89838CC40}\\AppOptions",
	"/sp" );
MyWriteKey(
	"HKCU\\SOFTWARE\\Classes\\CLSID\\{7B245014-8540-4190-936A-D0B89838CC40}\\ShowExisting",
	"Y" );

WScript.Quit( 0 );

// ****************************************************************************

function MyWriteKey ( strKey, strValue )
{
	try { WshShell.RegWrite( strKey, strValue, "REG_SZ" ); }
	catch ( err ) {}
}

// ****************************************************************************
