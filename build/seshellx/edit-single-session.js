// ****************************************************************************

var WshShell = WScript.CreateObject( "WScript.Shell" );

MyWriteKey(
	"HKCU\\SOFTWARE\\Classes\\CLSID\\{6A06A71E-CFFA-444C-B347-1B7898C9AB7B}\\",
	"EditWithSlickEdit" );
MyWriteKey(
	"HKCU\\SOFTWARE\\Classes\\CLSID\\{6A06A71E-CFFA-444C-B347-1B7898C9AB7B}\\AppOptions",
	"" );
MyWriteKey(
	"HKCU\\SOFTWARE\\Classes\\CLSID\\{6A06A71E-CFFA-444C-B347-1B7898C9AB7B}\\ShowExisting",
	"N" );

WScript.Quit( 0 );

// ****************************************************************************

function MyWriteKey ( strKey, strValue )
{
	try { WshShell.RegWrite( strKey, strValue, "REG_SZ" ); }
	catch ( err ) {}
}

// ****************************************************************************
