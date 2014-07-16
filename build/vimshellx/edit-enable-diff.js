// ****************************************************************************

var WshShell = WScript.CreateObject( "WScript.Shell" );

MyWriteKey(
	"HKCU\\SOFTWARE\\Classes\\CLSID\\{95FF2471-55D2-4B22-A352-B1299F199C29}\\",
	"EditWithVim" );
MyWriteKey(
	"HKCU\\SOFTWARE\\Classes\\CLSID\\{95FF2471-55D2-4B22-A352-B1299F199C29}\\ShowDiff",
	"Y" );

WScript.Quit( 0 );

// ****************************************************************************

function MyWriteKey ( strKey, strValue )
{
	try { WshShell.RegWrite( strKey, strValue, "REG_SZ" ); }
	catch ( err ) {}
}

// ****************************************************************************
