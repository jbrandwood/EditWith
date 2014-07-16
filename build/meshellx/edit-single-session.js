// ****************************************************************************

var WshShell = WScript.CreateObject( "WScript.Shell" );

MyWriteKey(
	"HKCU\\SOFTWARE\\Classes\\CLSID\\{762DB251-9E8E-44F5-B505-1770CEFBEB5D}\\",
	"EditWithMultiEdit" );
MyWriteKey(
	"HKCU\\SOFTWARE\\Classes\\CLSID\\{762DB251-9E8E-44F5-B505-1770CEFBEB5D}\\AppOptions",
	"/NoSplash" );
MyWriteKey(
	"HKCU\\SOFTWARE\\Classes\\CLSID\\{762DB251-9E8E-44F5-B505-1770CEFBEB5D}\\ShowExisting",
	"N" );

WScript.Quit( 0 );

// ****************************************************************************

function MyWriteKey ( strKey, strValue )
{
	try { WshShell.RegWrite( strKey, strValue, "REG_SZ" ); }
  catch ( err ) {}
}

// ****************************************************************************
