// ****************************************************************************

var WshShell = WScript.CreateObject( "WScript.Shell" );

MyWriteKey(
	"HKCU\\SOFTWARE\\Classes\\CLSID\\{6A06A71E-CFFA-444C-B347-1B7898C9AB7B}\\",
	"EditWithSlickEdit" );
MyWriteKey(
	"HKCU\\SOFTWARE\\Classes\\CLSID\\{6A06A71E-CFFA-444C-B347-1B7898C9AB7B}\\AppOptions",
	"+new" );
MyWriteKey(
	"HKCU\\SOFTWARE\\Classes\\CLSID\\{6A06A71E-CFFA-444C-B347-1B7898C9AB7B}\\ShowExisting",
	"Y" );

WshShell.Popup(
  "For this to work properly, you will need to go to SlickEdit's\n"
  + "\"Tools->Options->Application Options->Auto Restore\" menu\n"
  + "and set the \"Auto Restore files\" and \"Auto Restore workspace\"\n"
  + "options to \"OFF\".",
  0, "IMPORTANT - Please read!", 64 );

WScript.Quit( 0 );

// ****************************************************************************

function MyWriteKey ( strKey, strValue )
{
	try { WshShell.RegWrite( strKey, strValue, "REG_SZ" ); }
	catch ( err ) {}
}

// ****************************************************************************
