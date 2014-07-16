// ****************************************************************************

var WshShell = WScript.CreateObject( "WScript.Shell" );

MyWriteKey(
	"HKCU\\SOFTWARE\\Classes\\CLSID\\{7F38663E-04FA-4C75-8FAA-B5A73C8B9131}\\",
	"EditWithKomodo" );
MyWriteKey(
	"HKCU\\SOFTWARE\\Classes\\CLSID\\{7F38663E-04FA-4C75-8FAA-B5A73C8B9131}\\AppOptions",
	"-n" );
MyWriteKey(
	"HKCU\\SOFTWARE\\Classes\\CLSID\\{7F38663E-04FA-4C75-8FAA-B5A73C8B9131}\\ShowExisting",
	"Y" );

WScript.Quit( 0 );

// ****************************************************************************

function MyWriteKey ( strKey, strValue )
{
	try { WshShell.RegWrite( strKey, strValue, "REG_SZ" ); }
	catch ( err ) {}
}

// ****************************************************************************
