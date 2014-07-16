// ****************************************************************************

var WshShell = WScript.CreateObject( "WScript.Shell" );

var strExe = WshShell.ExpandEnvironmentStrings(
	"%ProgramFiles%\\Programmer's Notepad\\pn.exe" );

if ( Is64BitOS() )
{
	var objFSO = WScript.CreateObject( "Scripting.Filesystemobject" );
	if ( !objFSO.FileExists( strExe ) )
	{
		strExe = WshShell.ExpandEnvironmentStrings(
			"%ProgramFiles(x86)%\\Programmer's Notepad\\pn.exe" );
	}
}

MyWriteKey(
	"HKCU\\SOFTWARE\\Classes\\*\\Shell\\pnotepad\\",
  "Edit with Programmer's Notepad" );
MyWriteKey(
	"HKCU\\SOFTWARE\\Classes\\*\\Shell\\pnotepad\\command\\",
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

function Is64BitOS ()
{
	return ( GetObject(
		"winmgmts:root\\cimv2:Win32_Processor='cpu0'" ).AddressWidth == 64 );
}

// ****************************************************************************
