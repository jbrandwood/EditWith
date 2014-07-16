// ****************************************************************************

var WshShell = WScript.CreateObject( "WScript.Shell" );

var strExe = MyReadKey(
	"HKLM\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\EditPadPro7.exe\\" );

MyWriteKey(
	"HKCU\\SOFTWARE\\Classes\\*\\shell\\editpad\\",
  "Open with &EditPad" );
MyWriteKey(
	"HKCU\\SOFTWARE\\Classes\\*\\shell\\editpad\\command\\",
  '\"' + strExe + '\" \"%1\"'
 );

WScript.Quit( 0 );

// ****************************************************************************

function MyWriteKey ( strKey, strValue )
{
	try { WshShell.RegWrite( strKey, strValue, "REG_SZ" ); }
	catch ( err ) {}
}

// ****************************************************************************

function MyReadKey ( strKey )
{
	try { return WshShell.RegRead( strKey ); }
	catch ( err ) {}
}

// ****************************************************************************
