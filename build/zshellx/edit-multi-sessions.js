// ****************************************************************************

var WshShell = WScript.CreateObject( "WScript.Shell" );

MyWriteKey(
	"HKCU\\SOFTWARE\\Classes\\CLSID\\{8A854BCF-E3A3-4173-A1EB-D2FD9BE285F0}\\",
	"EditWithZeus" );
MyWriteKey(
	"HKCU\\SOFTWARE\\Classes\\CLSID\\{8A854BCF-E3A3-4173-A1EB-D2FD9BE285F0}\\AppOptions",
	"-s -n -vn-" );
MyWriteKey(
	"HKCU\\SOFTWARE\\Classes\\CLSID\\{8A854BCF-E3A3-4173-A1EB-D2FD9BE285F0}\\ShowExisting",
	"Y" );

WScript.Quit( 0 );

// ****************************************************************************

function MyWriteKey ( strKey, strValue )
{
	try { WshShell.RegWrite( strKey, strValue, "REG_SZ" ); }
	catch ( err ) {}
}

// ****************************************************************************
