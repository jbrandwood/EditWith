// ****************************************************************************

var WshShell = WScript.CreateObject( "WScript.Shell" );

var strExe = WshShell.ExpandEnvironmentStrings(
	"%ProgramFiles%\\Zeus\\Zeus.exe" );

if ( Is64BitOS() )
{
	var objFSO = WScript.CreateObject( "Scripting.Filesystemobject" );
	if ( !objFSO.FileExists( strExe ) )
	{
		strExe = WshShell.ExpandEnvironmentStrings(
			"%ProgramFiles(x86)%\\Zeus\\Zeus.exe" );
	}
}

MyWriteKey(
	"HKCU\\SOFTWARE\\Classes\\*\\shell\\ZeusEditor\\",
  "Zeus Editor" );
MyWriteKey(
	"HKCU\\SOFTWARE\\Classes\\*\\shell\\ZeusEditor\\command\\",
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
