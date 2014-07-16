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

if ( Is64BitOS() )
{
	MyWriteKey(
		"HKLM\\SOFTWARE\\Classes\\*\\shellex\\ContextMenuHandlers\\ANotepad++64\\",
		"{B298D29A-A6ED-11DE-BA8C-A68E55D89593}" );
} else {
	MyWriteKey(
		"HKLM\\SOFTWARE\\Classes\\*\\shellex\\ContextMenuHandlers\\ANotepad++\\",
		"{00F3C2EC-A6EE-11DE-A03A-EF8F55D89593}" );
}

WScript.Quit( 0 );

// ****************************************************************************

function MyWriteKey ( strKey, strValue )
{
	try { WshShell.RegWrite( strKey, strValue, "REG_SZ" ); }
	catch ( err ) {}
}

// ****************************************************************************

function Is64BitOS ()
{
	return ( GetObject(
		"winmgmts:root\\cimv2:Win32_Processor='cpu0'" ).AddressWidth == 64 );
}

// ****************************************************************************
