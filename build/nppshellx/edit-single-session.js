// ****************************************************************************

var WshShell = WScript.CreateObject( "WScript.Shell" );

MyWriteKey(
	"HKCU\\SOFTWARE\\Classes\\CLSID\\{1D1E7DAD-E970-4D20-9843-908C9CF89CE2}\\",
	"EditWithNPP" );
MyWriteKey(
	"HKCU\\SOFTWARE\\Classes\\CLSID\\{1D1E7DAD-E970-4D20-9843-908C9CF89CE2}\\AppOptions",
	"" );
MyWriteKey(
	"HKCU\\SOFTWARE\\Classes\\CLSID\\{1D1E7DAD-E970-4D20-9843-908C9CF89CE2}\\ShowExisting",
	"N" );

WScript.Quit( 0 );

// ****************************************************************************

function MyWriteKey ( strKey, strValue )
{
	try { WshShell.RegWrite( strKey, strValue, "REG_SZ" ); }
	catch ( err ) {}
}

// ****************************************************************************
