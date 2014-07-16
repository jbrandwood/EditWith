// ****************************************************************************
// Run this script as "Administrator"

if ( WScript.Arguments.length == 0 )
{
	var objShell = WScript.CreateObject( "Shell.Application" );
	objShell.ShellExecute( "wscript.exe",
		'"' + WScript.ScriptFullName + '"' + " uac", "", "runas", 1 );
	WScript.Quit( 0 );
}

var WshShell = WScript.CreateObject( "WScript.Shell" );

// var strExe = MyReadKey(
//	"HKLM\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\EditPadPro7.exe\\" );

MyWriteKey(
	"HKLM\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\komodo.exe\\",
	"C:\\Program Files\\ActiveState Komodo Edit 8\\lib\\mozilla\\komodo.exe" );
MyWriteKey(
	"HKLM\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\komodo.exe\\Path",
	"C:\\Program Files\\ActiveState Komodo Edit 8\\lib\\mozilla" );

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
