// ****************************************************************************

var WshShell = WScript.CreateObject( "WScript.Shell" );

MyWriteKey(
	"HKCU\\SOFTWARE\\Classes\\CLSID\\{ADE88F8C-82EC-4C30-941F-1240656F4EC8}\\",
	"EditWithUEStudio" );
MyWriteKey(
	"HKCU\\SOFTWARE\\Classes\\CLSID\\{ADE88F8C-82EC-4C30-941F-1240656F4EC8}\\AppOptions",
	"/fni" );
MyWriteKey(
	"HKCU\\SOFTWARE\\Classes\\CLSID\\{ADE88F8C-82EC-4C30-941F-1240656F4EC8}\\ShowExisting",
	"Y" );

WScript.Quit( 0 );

// ****************************************************************************

function MyWriteKey ( strKey, strValue )
{
	try { WshShell.RegWrite( strKey, strValue, "REG_SZ" ); }
	catch ( err ) {}
}

// ****************************************************************************
