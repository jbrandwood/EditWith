// ****************************************************************************

var WshShell = WScript.CreateObject( "WScript.Shell" );

MyWriteKey(
	"HKCU\\SOFTWARE\\Classes\\CLSID\\{4A169362-D7BC-4428-9E21-D998BD37807F}\\",
	"EditWithTextPad" );
MyWriteKey(
	"HKCU\\SOFTWARE\\Classes\\CLSID\\{4A169362-D7BC-4428-9E21-D998BD37807F}\\AppOptions",
	"" );
MyWriteKey(
	"HKCU\\SOFTWARE\\Classes\\CLSID\\{4A169362-D7BC-4428-9E21-D998BD37807F}\\ShowExisting",
	"N" );

WScript.Quit( 0 );

// ****************************************************************************

function MyWriteKey ( strKey, strValue )
{
	try { WshShell.RegWrite( strKey, strValue, "REG_SZ" ); }
	catch ( err ) {}
}

// ****************************************************************************
