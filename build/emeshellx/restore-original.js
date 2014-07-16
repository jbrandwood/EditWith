// ****************************************************************************

var WshShell = WScript.CreateObject( "WScript.Shell" );

if ( Is64BitOS() )
{
	MyWriteKey(
		"HKCU\\SOFTWARE\\Classes\\*\\shellex\\ContextMenuHandlers\\EmEditor\\",
		"{D4D48C93-BDC7-4E76-B530-2E4D13B0150F}" );
} else {
	MyWriteKey(
		"HKCU\\SOFTWARE\\Classes\\*\\shellex\\ContextMenuHandlers\\EmEditor\\",
		"{DFA0CC7F-D36B-47D1-8EF5-415C1DA53F57}" );
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
