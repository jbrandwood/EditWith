// ****************************************************************************

var WshShell = WScript.CreateObject( "WScript.Shell" );

MyWriteKey(
	"HKCU\\SOFTWARE\\Classes\\CLSID\\{36B40FAF-667E-4B19-8F41-DFEB32A370DC}\\",
	"EditWithSublimeText" );
MyWriteKey(
	"HKCU\\SOFTWARE\\Classes\\CLSID\\{36B40FAF-667E-4B19-8F41-DFEB32A370DC}\\AppOptions",
	"-n" );
MyWriteKey(
	"HKCU\\SOFTWARE\\Classes\\CLSID\\{36B40FAF-667E-4B19-8F41-DFEB32A370DC}\\ShowExisting",
	"Y" );

WScript.Quit( 0 );

// ****************************************************************************

function MyWriteKey ( strKey, strValue )
{
	try { WshShell.RegWrite( strKey, strValue, "REG_SZ" ); }
	catch ( err ) {}
}

// ****************************************************************************
