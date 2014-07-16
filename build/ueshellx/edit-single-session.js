// ****************************************************************************

var WshShell = WScript.CreateObject( "WScript.Shell" );

MyWriteKey(
	"HKCU\\SOFTWARE\\Classes\\CLSID\\{2044570C-9710-4C0B-9107-87C082C37B6E}\\",
	"EditWithUltraEdit" );
MyWriteKey(
	"HKCU\\SOFTWARE\\Classes\\CLSID\\{2044570C-9710-4C0B-9107-87C082C37B6E}\\AppOptions",
	"" );
MyWriteKey(
	"HKCU\\SOFTWARE\\Classes\\CLSID\\{2044570C-9710-4C0B-9107-87C082C37B6E}\\ShowExisting",
	"N" );

WScript.Quit( 0 );

// ****************************************************************************

function MyWriteKey ( strKey, strValue )
{
	try { WshShell.RegWrite( strKey, strValue, "REG_SZ" ); }
	catch ( err ) {}
}

// ****************************************************************************
