// ****************************************************************************

var WshShell = WScript.CreateObject( "WScript.Shell" );

MyWriteKey(
	"HKCU\\SOFTWARE\\Classes\\CLSID\\{6BD977B9-96E3-478C-B4EE-1DE777375A1A}\\",
	"EditWithPN2" );
MyWriteKey(
	"HKCU\\SOFTWARE\\Classes\\CLSID\\{6BD977B9-96E3-478C-B4EE-1DE777375A1A}\\AppOptions",
	"--allowmulti" );
MyWriteKey(
	"HKCU\\SOFTWARE\\Classes\\CLSID\\{6BD977B9-96E3-478C-B4EE-1DE777375A1A}\\ShowExisting",
	"Y" );

WScript.Quit( 0 );

// ****************************************************************************

function MyWriteKey ( strKey, strValue )
{
	try { WshShell.RegWrite( strKey, strValue, "REG_SZ" ); }
	catch ( err ) {}
}

// ****************************************************************************
