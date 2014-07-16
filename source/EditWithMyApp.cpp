// ****************************************************************************
// ****************************************************************************
//
// EditWithMyApp.cpp : Implementation of CEditWithMyApp shell extension object
//
//  Copyright John Brandwood 2014.
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//    http://www.boost.org/LICENSE_1_0.txt)
//

#include "stdafx.h"
#include "config.h"
#include "resource.h"
#include "EditWithMyApp.h"
#include "ResourceFuncs.h"

#include <wchar.h>
#include <shlwapi.h>

//
// WDK 7.1.0 doesn't have psapi.h, so just define what we need.
//

extern "C" DWORD WINAPI GetProcessImageFileNameW (
  HANDLE hProcess, LPWSTR lpImageFileName, DWORD nSize );

//
// MY_COM_CLASS and MY_COM_TITLE are defined in config.h
//

const CLSID CLSID_MyExtensionClass = MY_COM_CLASS;
const wchar_t * szMyExtensionTitle = MY_COM_TITLE;

//
// Each shell extension supplies these in config.cpp
//

extern int SelectMyIcon (
  int iWantSz, bool &fCenterIcon, bool &fHQBScaling );

extern void SplitTitleString (
  wchar_t * pString, wchar_t * &pHead, wchar_t * &pFile, wchar_t * &pTail );

//
// Static class members so that we can cache the icon bitmap.
//

HBITMAP CEditWithMyApp::s_hIconBitmap = NULL;
int     CEditWithMyApp::s_iIconW      = 0;
int     CEditWithMyApp::s_iIconH      = 0;

//
// ****************************************************************************
//
// Separate the actual functional implementation here from the DLL & COM code.
//

BOOL CEditWithMyApp::OnCreation( void )
{
  this->m_pDataObject        = NULL;
  this->m_cStgMedium.hGlobal = NULL;
  this->m_hDrop              = NULL;

  this->m_uNumExistingWindows = 0;

  this->GetMyOptions();

#ifdef _DEBUG
  // The default setting is MAKELANGID( LANG_NEUTRAL, SUBLANG_NEUTRAL )
  // which will use the current process's default language.
  // You can override it here for testing different languages.
//SetMyResourceStringLanguage( MAKELANGID( LANG_ENGLISH, SUBLANG_ENGLISH_US ) );
//SetMyResourceStringLanguage( MAKELANGID( LANG_GERMAN,  SUBLANG_GERMAN ) );
#endif

  return TRUE;
}

//
// ****************************************************************************
//
// Separate the actual functional implementation here from the DLL & COM code.
//

void CEditWithMyApp::OnDestruction ( void )
{
  this->FreeDataObject();
}

//
// ****************************************************************************
//

void CEditWithMyApp::GetMyOptions( void )
{
  // Start off with our default values.

  wcscpy_s(
    this->m_aAppOptions, ARRAYSIZE( this->m_aAppOptions ), MY_EXE_OPTS );

  #ifdef SINGLE_MY_APP_INSTANCE
    this->m_fShowExisting = false;
  #else
    this->m_fShowExisting = true;
  #endif

  #ifdef MY_DIFF_OPTS
    this->m_fShowDiff = true;
  #else
    this->m_fShowDiff = false;
  #endif

  #ifdef MY_HEX_OPTS
    this->m_fShowHex  = true;
  #else
    this->m_fShowHex  = false;
  #endif

  // Allow registry overrides in the HKCU key.

  LONG lResult;
  HKEY hKey;

  wchar_t szName [ 40 ];
  wchar_t szData [ OPTS_BUFFER_CHARS ];

  // Convert our extension's CLSID into string format for the registry.
  StringFromGUID2( CLSID_MyExtensionClass, szName, ARRAYSIZE( szName ) );

  wcscpy_s( szData, ARRAYSIZE( szData ), L"Software\\Classes\\CLSID\\" );
  wcscat_s( szData, ARRAYSIZE( szData ), szName );

  lResult = RegOpenKeyExW( HKEY_CURRENT_USER,
    szData, 0, KEY_QUERY_VALUE | STANDARD_RIGHTS_READ , &hKey );
  if ( lResult != ERROR_SUCCESS ) return;

  int iIndex = 0;

  for (;;)
  {
    DWORD dwName = ARRAYSIZE( szName ) - 1;
    DWORD dwData =    sizeof( szData ) - 2;
    DWORD dwType;

    lResult = RegEnumValueW( hKey, iIndex++,
      szName, &dwName, 0, &dwType, (LPBYTE) szData, &dwData );

    if ( lResult != ERROR_SUCCESS || dwType != REG_SZ ) break;

    szName[ dwName      ] = '\0';
    szData[ dwData >> 1 ] = '\0';

    if ( wcscmp( szName, L"AppOptions" ) == 0 )
    {
      wcscpy_s(
        this->m_aAppOptions, ARRAYSIZE( this->m_aAppOptions ), szData );
      continue;
    }
    if ( wcscmp( szName, L"ShowExisting" ) == 0 )
    {
      if ( szData[0] == L'Y' )
        this->m_fShowExisting = true;
      else
        this->m_fShowExisting = false;
      continue;
    }

#ifdef MY_DIFF_OPTS
    if ( wcscmp( szName, L"ShowDiff" ) == 0 )
    {
      if ( szData[0] == L'Y' )
        this->m_fShowDiff = true;
      else
        this->m_fShowDiff = false;
      continue;
    }
#endif

#ifdef MY_HEX_OPTS
    if ( wcscmp( szName, L"ShowHex" ) == 0 )
    {
      if ( szData[0] == L'Y' )
        this->m_fShowHex = true;
      else
        this->m_fShowHex = false;
      continue;
    }
#endif
  }

  RegCloseKey( hKey );
}

//
// ****************************************************************************
//

STDMETHODIMP CEditWithMyApp::Initialize (
  LPCITEMIDLIST pidlFolder, LPDATAOBJECT pDataObject, HKEY hProgID )
{
  // Apparently, handlers can be initialized multiple times, so make absolutely
  // sure that we don't keep hold of a reference to a stale data object.
  this->FreeDataObject();

  // Save the COM data object (i.e. the CF_HDROP file list) for later use.
  if ( pDataObject )
  {
    this->m_pDataObject = pDataObject;
    this->m_pDataObject->AddRef();
  }

  return S_OK;
}

//
// ****************************************************************************
//

STDMETHODIMP CEditWithMyApp::QueryContextMenu (
  HMENU hMenu, UINT uMenuIndex, UINT uCmdFirst, UINT uCmdLast, UINT uFlags )
{
  // Don't respond if CMF_DEFAULTONLY is set.
  if ( uFlags & CMF_DEFAULTONLY )
  {
    return MAKE_HRESULT( SEVERITY_SUCCESS, FACILITY_NULL, 0 );
  }

  // Don't respond if we don't have a file list.
  if ( !this->GetFileList() )
  {
    return MAKE_HRESULT( SEVERITY_SUCCESS, FACILITY_NULL, 0 );
  }

#ifdef MY_DIFF_OPTS
  // Limit when we show the "diff" menu item.
  if ( this->m_uNumFilesInList < 2 || this->m_uNumFilesInList > MAXIMUM_DIFF )
  {
    this->m_fShowDiff = false;
  }
#endif

  // Load up the bitmap icon to display in the menu.
  HBITMAP hIconBitmap = this->GetMenuIcon();

  MENUITEMINFOW cMii;

  wchar_t aTmp [256];

  // Constant values.
  cMii.cbSize     = sizeof(MENUITEMINFO);
  cMii.dwTypeData = aTmp;

  UINT uBaseMask = MIIM_STRING | MIIM_ID;

  if ( g_fRunningOnXP )
  {
    // Use HandleMenuMsg2() callback to draw.
    uBaseMask |= MIIM_BITMAP ;
    cMii.hbmpItem = HBMMENU_CALLBACK;
  }
  else
  {
    // Let Windows draw the icon.
    uBaseMask |= MIIM_CHECKMARKS;
    cMii.hbmpChecked   = hIconBitmap;
    cMii.hbmpUnchecked = hIconBitmap;
  }

  // Keep track of how many items we add.
  UINT uCmdIndex = uCmdFirst;

  // L"Edit with Zeus"
  cMii.fMask = uBaseMask;
  cMii.wID   = uCmdIndex++;
  cMii.cch   = GetMyResourceString( IDS_MENU_NEWAPP, aTmp, ARRAYSIZE( aTmp ) );
  InsertMenuItemW( hMenu, uMenuIndex++, TRUE, &cMii );

#ifdef MY_HEX_OPTS
  if ( this->m_fShowHex )
  {
    // L"Edit with PSPad (hex)"
    cMii.fMask = uBaseMask;
    cMii.wID   = uCmdIndex++;
    cMii.cch   = GetMyResourceString( IDS_MENU_HEX, aTmp, ARRAYSIZE( aTmp ) );
    InsertMenuItemW( hMenu, uMenuIndex++, TRUE, &cMii );
  }
#endif

  // We need to know if there's an existing session, even if we're in
  // single-session mode so that we can drop large file lists.
  this->m_uNumExistingWindows = 0;
  EnumWindows( this->EnumWindowCallback, (LPARAM) this );

  // Add any existing instances to a submenu.
  if ( this->m_fShowExisting && this->m_uNumExistingWindows )
  {
    HMENU hSubMenu = CreatePopupMenu();

    for ( UINT i = 0; i < this->m_uNumExistingWindows; i++ )
    {
      wchar_t aTempPath [PATH_BUFFER_CHARS];

      // Get the window's title and fake one if it doesn't have one.
      if ( GetWindowTextW( this->m_aWindowList[i], aTempPath, ARRAYSIZE( aTempPath ) ) == 0 )
      {
        wcscpy_s( aTmp, ARRAYSIZE( aTmp ), L"<unknown>" );
      }
      else
      {
        wchar_t * pHead = NULL;
        wchar_t * pFile = NULL;
        wchar_t * pTail = NULL;

        // Call editor-specific func in "config.cpp" to split it up.
        SplitTitleString( aTempPath, pHead, pFile, pTail );

        if ( pFile == pTail )
        {
          wcscpy_s( aTmp, ARRAYSIZE( aTmp ), pHead );
        }
        else
        {
          wchar_t aSrcFile [ MAX_PATH + 1 ];
          wchar_t aOutFile [ MAX_PATH + 1 ];

          if ( pTail - pFile > MAX_PATH ) { pFile = pTail - MAX_PATH; }

          wmemcpy( aSrcFile, pFile, pTail - pFile );

          aSrcFile[ pTail - pFile ] = L'\0';

          if ( !PathCompactPathEx( aOutFile, aSrcFile, MAX_FILE_DISPLAY_WIDTH, 0 ) )
          {
            wcscpy_s( aOutFile, ARRAYSIZE( aOutFile ), L"..." );
          }

          wmemcpy( aTmp, pHead, pFile - pHead );
          aTmp[ pFile - pHead ] = L'\0';
          wcscat_s( aTmp, ARRAYSIZE( aTmp ), aOutFile );
          wcscat_s( aTmp, ARRAYSIZE( aTmp ), pTail );
        }
      }

      AppendMenuW( hSubMenu, MF_STRING, uCmdIndex++, aTmp );
    }

    // L"Edit in existing Zeus session"
    // This needs an MIIM_ID, or XP won't draw the bitmap icon.
    cMii.fMask    = uBaseMask | MIIM_SUBMENU;
    cMii.wID      = uCmdIndex++;
    cMii.cch      = GetMyResourceString( IDS_MENU_OLDAPP, aTmp, ARRAYSIZE( aTmp ) );
    cMii.hSubMenu = hSubMenu;
    InsertMenuItemW( hMenu, uMenuIndex++, TRUE, &cMii );
  }

#ifdef MY_DIFF_OPTS
  if ( this->m_fShowDiff )
  {
    // L"Diff with PSPad"
    cMii.fMask = uBaseMask;
    cMii.wID   = uCmdIndex++;
    cMii.cch   = GetMyResourceString( IDS_MENU_DIFF, aTmp, ARRAYSIZE( aTmp ) );
    InsertMenuItemW( hMenu, uMenuIndex++, TRUE, &cMii );
  }
#endif

  // Tell Windows Explorer how many items with commands we've added.
  return MAKE_HRESULT( SEVERITY_SUCCESS, FACILITY_NULL, uCmdIndex - uCmdFirst );
}

//
// ****************************************************************************
//
// Only needed here for alpha-blended rendering of the icon on Windows XP.
//

STDMETHODIMP CEditWithMyApp::HandleMenuMsg (
  UINT uMsg, WPARAM wParam, LPARAM lParam )
{
  return ( this->HandleMenuMsg2( uMsg, wParam, lParam, NULL ) );
}

//
// ****************************************************************************
//
// Only needed here for alpha-blended rendering of the icon on Windows XP.
//

STDMETHODIMP CEditWithMyApp::HandleMenuMsg2 (
  UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT * pResult )
{
  switch ( uMsg )
  {
    case WM_MEASUREITEM:
    {
      MEASUREITEMSTRUCT * pMIS = (MEASUREITEMSTRUCT *) lParam;
      if ( pMIS )
      {
        pMIS->itemWidth  = 0;
        pMIS->itemHeight = this->s_iIconH;
        if ( pResult ) *pResult = TRUE;
      }
      break;
    }
    case WM_DRAWITEM:
    {
      DRAWITEMSTRUCT * pDIS = (DRAWITEMSTRUCT *) lParam;
      if ( pDIS && pDIS->CtlType == ODT_MENU )
      {
        if ( this->s_hIconBitmap )
        {
          HDC hDC = CreateCompatibleDC( pDIS->hDC );
          HGDIOBJ hLast = SelectObject( hDC, this->s_hIconBitmap );

          BLENDFUNCTION cBF;
          cBF.BlendOp     = AC_SRC_OVER;
          cBF.BlendFlags  = 0;
          cBF.AlphaFormat = AC_SRC_ALPHA;
          cBF.SourceConstantAlpha = 0xFF;

          AlphaBlend(
            pDIS->hDC, 2, 2, this->s_iIconW, this->s_iIconH,
            hDC, 0, 0, this->s_iIconW, this->s_iIconH, cBF );

          SelectObject( hDC, hLast );
          DeleteDC ( hDC );
        }
        if ( pResult ) *pResult = TRUE;
      }
      break;
    }
  }

  return S_OK;
}

//
// ****************************************************************************
//

void CEditWithMyApp::FreeDataObject ( void )
{
  this->m_uNumFilesInList = 0;
  this->m_uNumCharsInList = 0;

  if ( this->m_hDrop )
  {
    this->m_hDrop = NULL;
    GlobalUnlock( this->m_cStgMedium.hGlobal );
  }
  if ( this->m_cStgMedium.hGlobal )
  {
    ReleaseStgMedium( &this->m_cStgMedium );
    this->m_cStgMedium.hGlobal = NULL;
  }
  if ( this->m_pDataObject )
  {
    this->m_pDataObject->Release();
    this->m_pDataObject = NULL;
  }
}

//
// ****************************************************************************
//

bool CEditWithMyApp::GetFileList ( void )
{
  // Only do this once.
  if ( this->m_hDrop ) return true;

  FORMATETC cFmt = { CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };

  this->m_cStgMedium.tymed          = TYMED_HGLOBAL; 
  this->m_cStgMedium.hGlobal        = NULL;
  this->m_cStgMedium.pUnkForRelease = NULL;

  // Look for CF_HDROP data in the IDataObject that Windows Explorer sent us.
  if (( this->m_pDataObject ) &&
      ( this->m_pDataObject->GetData( &cFmt, &this->m_cStgMedium ) == S_OK ))
  {
    this->m_hDrop = (HDROP) GlobalLock( this->m_cStgMedium.hGlobal );

    if ( this->m_hDrop )
    {
      // Find out how many files we've been passed.
      this->m_uNumCharsInList = 0;
      this->m_uNumFilesInList = DragQueryFileW( this->m_hDrop, 0xFFFFFFFF, NULL, 0 );

      if ( this->m_uNumFilesInList != 0 )
      {
        // Find out how many chars in the list of files.
        for ( UINT i = 0; i < this->m_uNumFilesInList; i++ )
        {
          this->m_uNumCharsInList += DragQueryFileW( this->m_hDrop, i, NULL, 0 );
        }
      }

      return true;
    }
  }

  // Something went wrong!
  this->FreeDataObject();
  return false;
}

//
// ****************************************************************************
//
// Windows 7 "Windows Classic" Theme (also Windows XP) ...
//
//   Windows Smaller Text 100% ( 96dpi) -> check mark icon is 13x13 pixels.
//   Windows Medium  Text 125% (120dpi) -> check mark icon is 17x17 pixels.
//   Windows Larger  Text 150% (144dpi) -> check mark icon is 21x21 pixels.
//   Windows Largest Text 200% (192dpi) -> check mark icon is 27x27 pixels.
//
// Windows 7 "Windows 7 Aero" Theme (also Windows Vista) ...
//
//   Windows Smaller Text 100% ( 96dpi) -> check mark icon is 15x15 pixels.
//   Windows Medium  Text 125% (120dpi) -> check mark icon is 19x19 pixels.
//   Windows Larger  Text 150% (144dpi) -> check mark icon is 25x25 pixels.
//   Windows Largest Text 200% (192dpi) -> check mark icon is 31x31 pixels.
//

HBITMAP CEditWithMyApp::GetMenuIcon ( void )
{
  // Find out the size of the icon that Windows would like us to display.
  int iCheckMarkW = GetSystemMetrics( SM_CXMENUCHECK );
  int iCheckMarkH = GetSystemMetrics( SM_CYMENUCHECK );

  HBITMAP hIconBitmap = NULL;

  // Return the cached copy if possible.
  if ( CEditWithMyApp::s_hIconBitmap != NULL )
  {
    if ( CEditWithMyApp::s_iIconH == iCheckMarkH )
      return CEditWithMyApp::s_hIconBitmap;

    hIconBitmap = CEditWithMyApp::s_hIconBitmap;
    CEditWithMyApp::s_hIconBitmap = NULL;
    DeleteObject( hIconBitmap );
    hIconBitmap = NULL;
  }

  ULONG_PTR pGdiplusToken;

  Gdiplus::GdiplusStartupInput cGdiplusStartupInput;

  if ( Gdiplus::GdiplusStartup(
    &pGdiplusToken, &cGdiplusStartupInput, NULL ) != Gdiplus::Ok ) return NULL;

  Gdiplus::Bitmap *   pOldIconBitmap  = NULL;
  Gdiplus::Bitmap *   pNewIconBitmap  = NULL;
  Gdiplus::Graphics * pGraphicsObject = NULL;

  bool fCenterIcon = false;
  bool fHQBScaling = true;

#ifdef GET_ICON_FROM_MY_EXE

  // Fixed size buffer. Faster, but yuk!
  BYTE pPixelData [ 32*32*4 ];
  if ( iCheckMarkW > 32 ) iCheckMarkW = 32;
  if ( iCheckMarkH > 32 ) iCheckMarkH = 32;

  wchar_t aExePathStr [ PATH_BUFFER_CHARS ];

  if ( this->LocateMyApp( aExePathStr, ARRAYSIZE( aExePathStr ) ) )
  {
    pOldIconBitmap =
      LoadMyAppIconAsBitmap( aExePathStr, pPixelData,
        SelectMyIcon( iCheckMarkW, fCenterIcon, fHQBScaling ) );
  }

#else

  int iResourceID = SelectMyIcon( iCheckMarkW, fCenterIcon, fHQBScaling );

  pOldIconBitmap =
    LoadMyBitmapResource( this->m_hDllInstance, MAKEINTRESOURCE( iResourceID ) );

#endif

  if ( pOldIconBitmap )
  {
    int iOldIconW = pOldIconBitmap->GetWidth();
    int iOldIconH = pOldIconBitmap->GetHeight();

    pNewIconBitmap =
      new Gdiplus::Bitmap( iCheckMarkW, iCheckMarkH,
          g_fRunningOnXP ? PixelFormat32bppARGB : PixelFormat32bppPARGB );

    if ( pNewIconBitmap )
    {
      pGraphicsObject = new Gdiplus::Graphics( pNewIconBitmap );

      if ( pGraphicsObject )
      {
        Gdiplus::Color cMenuBackground( (Gdiplus::ARGB) 0 );

        pGraphicsObject->Clear( cMenuBackground );

        pGraphicsObject->SetCompositingMode( Gdiplus::CompositingModeSourceOver );

        if ( iCheckMarkW < iOldIconW || iCheckMarkH < iOldIconH )
        {
          fCenterIcon = false;
        }

        if ( fCenterIcon )
        {
          pGraphicsObject->SetInterpolationMode( Gdiplus::InterpolationModeNearestNeighbor );

          pGraphicsObject->DrawImage( pOldIconBitmap,
            ( iCheckMarkW - iOldIconW ) >> 1,
            ( iCheckMarkH - iOldIconH ) >> 1, iOldIconW, iOldIconH );
        }
        else
        {
          pGraphicsObject->SetInterpolationMode(
            fHQBScaling ?
              Gdiplus::InterpolationModeHighQualityBicubic :
              Gdiplus::InterpolationModeBicubic );

          pGraphicsObject->DrawImage( pOldIconBitmap, 0, 0, iCheckMarkW, iCheckMarkH );
        }

        pNewIconBitmap->GetHBITMAP( cMenuBackground, &hIconBitmap );

        CEditWithMyApp::s_hIconBitmap = hIconBitmap;
        CEditWithMyApp::s_iIconW      = iCheckMarkW;
        CEditWithMyApp::s_iIconH      = iCheckMarkH;
      }
    }
  }

  delete pGraphicsObject;
  delete pNewIconBitmap;
  delete pOldIconBitmap;

  Gdiplus::GdiplusShutdown( pGdiplusToken );

  // Return the HBITMAP format icon that we've just created.
  return hIconBitmap;
}

//
// ****************************************************************************
//

void CEditWithMyApp::FreeStaticResources( void )
{
  if ( CEditWithMyApp::s_hIconBitmap )
  {
    DeleteObject( CEditWithMyApp::s_hIconBitmap );

    CEditWithMyApp::s_hIconBitmap = NULL;
  }
}

//
// ****************************************************************************
//

BOOL CALLBACK CEditWithMyApp::EnumWindowCallback (
  HWND hWindow, LPARAM lParam )
{
  // Ignore windows that are invisible or are disabled.
  if ( !IsWindowVisible( hWindow ) || !IsWindowEnabled( hWindow ) )
    return TRUE;

  // A buffer large enough to hold the text that we're looking for.
  wchar_t aTempString [ MAX_PATH ];

  // Ignore windows without a class name.
  if ( GetClassNameW( hWindow, aTempString, ARRAYSIZE( aTempString ) ) == 0 )
    return TRUE;

  // Ignore windows that aren't from our desired application.
  // For example, the VEDIT text editor uses the class name "vwCFrame".
  //
  // Optionally : Limit the length compared.
  //
  //   This is needed for some MFC-based programs that change the last digits
  //   of their "Afx:..." class name with every new window.

#ifdef MY_WIN_CLASS_LEN
  if ( wcsncmp( aTempString, MY_WIN_CLASS_STR, MY_WIN_CLASS_LEN ) != 0 )
    return TRUE;
#else
  if (  wcscmp( aTempString, MY_WIN_CLASS_STR ) != 0 )
    return TRUE;
#endif

  // Optionally : Check for a substring within the window title.
  //
  //   This is needed for some programs that use generic class names or use the
  //   same class name for different products.

#ifdef MY_WIN_TITLE_STR
  if ( GetWindowTextW( hWindow, aTempString, ARRAYSIZE( aTempString ) ) == 0 )
    return TRUE;

  if ( wcsstr( aTempString, MY_WIN_TITLE_STR ) == NULL )
    return TRUE;
#endif

  // Optionally : Check that the process image file matches our app.
  //
  //   This is slow, but can eliminate the last chance of false positives for
  //   apps that use generic class names.

#ifdef MY_EXE_IMAGE_CHK
  DWORD dwProcessID;
  if ( GetWindowThreadProcessId( hWindow, &dwProcessID ) == 0 )
    return TRUE;
  
  HANDLE hProcess =
    OpenProcess(
      g_fRunningOnXP ?
        PROCESS_QUERY_INFORMATION :
        PROCESS_QUERY_LIMITED_INFORMATION,
      FALSE, dwProcessID );

  if ( NULL == hProcess )
    return TRUE;

  DWORD dwCount =
    GetProcessImageFileNameW( hProcess, aTempString, ARRAYSIZE( aTempString ) );

  CloseHandle( hProcess );

  wchar_t * pTempString =
    aTempString + dwCount - ( ARRAYSIZE( MY_EXE_NAME ) - 1 );

  if ( pTempString < aTempString || wcscmp( pTempString, MY_EXE_NAME ) != 0 )
    return TRUE;
#endif

  // Save this window handle as an "existing session".

  CEditWithMyApp * pThis = reinterpret_cast<CEditWithMyApp *> ( lParam );

  pThis->m_aWindowList [ pThis->m_uNumExistingWindows ] = hWindow;

  if ( ++pThis->m_uNumExistingWindows < MAX_EXISTING_WINDOWS )
    return TRUE;
  else
    return FALSE;
}

//
// ****************************************************************************
//

char CEditWithMyApp::MenuCmdToAction( UINT uMenuCmd, UINT * pMenuCmd )
{
  // Turn the command index into an action.
  // This is rather ugly because of the various optional menu items.
  char iAction = '\0';

  for (;;)
  {
    if ( uMenuCmd == 0 )
    {
      iAction = 'N';
      break;
    }
    uMenuCmd -= 1;

#ifdef MY_HEX_OPTS
    if ( this->m_fShowHex )
    {
      if ( uMenuCmd == 0 )
      {
        iAction = 'H';
        break;
      }
      uMenuCmd -= 1;
    }
#endif

    if ( this->m_fShowExisting && this->m_uNumExistingWindows )
    {
      if ( uMenuCmd < this->m_uNumExistingWindows )
      {
        iAction = 'O';
        break;
      }
      // N.B. +1 to include the submenu itself.
      uMenuCmd -= ( this->m_uNumExistingWindows + 1 );
    }

#ifdef MY_DIFF_OPTS
    if ( this->m_fShowDiff )
    {
      if ( uMenuCmd == 0 )
      {
        iAction = 'D';
        break;
      }
      uMenuCmd -= 1;
    }
#endif

    break;
  }

  if ( pMenuCmd ) *pMenuCmd = uMenuCmd;

  return iAction;
}

//
// ****************************************************************************
//

HRESULT CEditWithMyApp::GetCommandString(
  UINT_PTR idCmd, UINT uFlags, UINT *pwReserved, LPSTR pszName, UINT cchMax )
{
  if ( !(uFlags & GCS_HELPTEXT) )
  {
    return E_INVALIDARG;
  }

  wchar_t aTmp [256];
  UINT    uLen;

  switch ( this->MenuCmdToAction( (UINT) idCmd, NULL ) )
  {
    case 'N':
      // L"Open the file(s) in XXX"
      uLen = GetMyResourceString( IDS_HELP_NEWAPP, aTmp, ARRAYSIZE( aTmp ) );
      break;

    case 'O':
      // L"Send the file(s) to the selected XXX session"
      uLen = GetMyResourceString( IDS_HELP_OLDAPP, aTmp, ARRAYSIZE( aTmp ) );
      break;

#ifdef MY_HEX_OPTS
    case 'H':
      // L"Edit the file(s) in XXX (hex mode)"
      uLen = GetMyResourceString( IDS_HELP_HEX, aTmp, ARRAYSIZE( aTmp ) );
      break;
#endif

#ifdef MY_DIFF_OPTS
    case 'D':
      // L"Diff the file(s) in XXX"
      uLen = GetMyResourceString( IDS_HELP_DIFF, aTmp, ARRAYSIZE( aTmp ) );
      break;
#endif

    default:
      return E_INVALIDARG;
  }
    
  if ( uFlags & GCS_UNICODE )
  {
    wcsncpy_s( reinterpret_cast<wchar_t *> (pszName), cchMax, aTmp, _TRUNCATE );
  }
  else
  {
    // Damn! ... wcstombs_s is not available on XP or XPx64.
    // wcstombs_s( NULL, pszName, cchMax, aTmp, _TRUNCATE );
    ZeroMemory( pszName, cchMax );
    wcstombs( pszName, aTmp, cchMax - 1 );
  }

  return S_OK;
}

//
// ****************************************************************************
//

HRESULT CEditWithMyApp::InvokeCommand(
  LPCMINVOKECOMMANDINFO pCmdInfo )
{
  // If lpVerb really points to a string, ignore this call and bail out.
  if ( 0 != HIWORD( pCmdInfo->lpVerb ) )
  {
    return E_INVALIDARG;
  }

  HRESULT hResult = S_OK;

  // The command index is relative to the items we added to the menu.
  UINT idCmd = LOWORD( pCmdInfo->lpVerb );

  // Now act upon what we've decided to do.
  switch ( this->MenuCmdToAction( idCmd, &idCmd ) )
  {
    case 'N':
      if ( this->m_fShowExisting || this->m_uNumExistingWindows == 0 )
      {
        this->SendFilesToNewAppInstance( pCmdInfo->hwnd, L"" );
        break;
      }
      // Single-session mode with an existing session? Then fall through ...
      idCmd = 0;

    case 'O':
      this->SendFilesToOldAppInstance( pCmdInfo->hwnd, this->m_aWindowList[ idCmd ] );
      break;

#ifdef MY_HEX_OPTS
    case 'H':
      this->SendFilesToNewAppInstance( pCmdInfo->hwnd, MY_HEX_OPTS );
      break;
#endif

#ifdef MY_DIFF_OPTS
    case 'D':
      this->SendFilesToNewAppInstance( pCmdInfo->hwnd, MY_DIFF_OPTS );
      break;
#endif

    default:
      hResult = E_INVALIDARG;
  }

  return hResult;
}

//
// ****************************************************************************
//

HRESULT CEditWithMyApp::SendFilesToNewAppInstance(
  HWND hParent, const wchar_t * szExtraOpts )
{
  wchar_t aExePathStr [PATH_BUFFER_CHARS];

  if ( !this->LocateMyApp( aExePathStr, ARRAYSIZE( aExePathStr ) ) )
  {
    #ifdef _DEBUG
      MessageBoxW(
        hParent,
        L"Unable to locate " MY_EXE_NAME L" in the registry!",
        MY_COM_TITLE L" - Error",
        MB_ICONERROR | MB_OK);
    #endif
    return FALSE;
  }

  wchar_t aOptionsStr [OPTS_BUFFER_CHARS+1];

  wcscpy_s( aOptionsStr, ARRAYSIZE( aOptionsStr ), this->m_aAppOptions );
  wcscat_s( aOptionsStr, ARRAYSIZE( aOptionsStr ), szExtraOpts );

#ifdef SKIP_OPTS_IF_NO_INSTANCE
  // Nasty hack for Komodo Edit / Komodo IDE feature (bug?).
  // They open 2 windows if opened with "-n" when not already running.
  // Also for Sublime Text which restores files in the first session opened.
  if ( this->m_uNumExistingWindows == 0 )
  {
    *aOptionsStr = '\0';
  }
#endif

  UINT uExePathLen = (UINT) wcslen( aExePathStr );
  UINT uOptionsLen = (UINT) wcslen( aOptionsStr );

  // Workspace needed = Exec length + String length + a quote-quote-space for each file.
  UINT uBufNeed = uExePathLen + uOptionsLen
    + this->m_uNumCharsInList + ((this->m_uNumFilesInList + 1) * 3) + 1;

  // If there are too many files for the command line, then we'll just open up
  // a new instance and then drag-n-drop the files on it once it's running.
  bool fFilesOnCLI = true;
  if ( uBufNeed > 32760 )
  {
#ifdef NO_INITIAL_DROP_ON_2ND
    // SlickEdit and PSPad will only open their 1st session without a file
    // being specified on the command line (which we need for the d-n-d).
    if ( this->m_uNumExistingWindows )
    {
      MessageBoxW(
        hParent,
        L"Too many files for new editor session!",
        MY_COM_TITLE L" - Error",
        MB_ICONERROR | MB_OK);
      return FALSE;
    }
#endif
    fFilesOnCLI = false;
    uBufNeed = uExePathLen + uOptionsLen + 4;
  }

  // Allocate a buffer and create a command line from the file list.
  wchar_t * pCommandLine =
    reinterpret_cast<wchar_t *> ( malloc( uBufNeed * sizeof(wchar_t) ) );

  if ( NULL == pCommandLine )
  {
    #ifdef _DEBUG
      MessageBoxW(
        hParent,
        L"Unable to allocate memory!",
        MY_COM_TITLE L" - Error",
        MB_ICONERROR | MB_OK);
    #endif
    return FALSE;
  }

  wchar_t * pBufData = pCommandLine;
  UINT      uBufLeft = uBufNeed;

  BOOL bBuiltOK = TRUE;
  pBufData[0] = L'\"';
  pBufData += 1;
  uBufLeft -= 1;
  wcscpy_s( pBufData, uBufLeft, aExePathStr );
  pBufData += uExePathLen;
  uBufLeft -= uExePathLen;
  pBufData[0] = L'\"';
  pBufData[1] = L' ';
  pBufData += 2;
  uBufLeft -= 2;
  wcscpy_s( pBufData, uBufLeft, aOptionsStr );
  pBufData += uOptionsLen;
  uBufLeft -= uOptionsLen;
  pBufData[0] = L' ';
  pBufData += 1;
  uBufLeft -= 1;

  if ( fFilesOnCLI )
  {
    for ( UINT i = 0; i < this->m_uNumFilesInList; i++ )
    {
      pBufData[0] = L'\"';
      pBufData += 1;
      uBufLeft -= 1;
      UINT uCurSize = DragQueryFileW( this->m_hDrop, i, pBufData, uBufLeft );
      if (uCurSize == 0)
      {
        bBuiltOK = FALSE;
        break;
      }
      pBufData += uCurSize;
      uBufLeft -= uCurSize;
      pBufData[0] = L'\"';
      pBufData[1] = L' ';
      pBufData += 2;
      uBufLeft -= 2;
    }
  }

  // Overwrite the final <space> with a terminating NULL;
  *( pBufData - 1 ) = 0;

  if ( !bBuiltOK || uBufLeft != 0 )
  {
    #ifdef _DEBUG
      MessageBoxW(
        hParent,
        L"Failed to assemble command line!",
        MY_COM_TITLE L" - Error",
        MB_ICONERROR | MB_OK);
    #endif
    return FALSE;
  }

  PROCESS_INFORMATION cPI;
  STARTUPINFOW        cSI;

  ZeroMemory( &cSI, sizeof( cSI ) );
  cSI.cb = sizeof( cSI );

  // Start the child process.
  if ( !CreateProcessW(
    NULL,          // No module name (use command line).
    pCommandLine,  // Command line.
    NULL,          // Process handle not inheritable.
    NULL,          // Thread handle not inheritable.
    FALSE,         // Set handle inheritance to FALSE.
    CREATE_UNICODE_ENVIRONMENT,
    NULL,          // Use parent's environment block.
    NULL,          // Use parent's starting directory.
    &cSI,          // Pointer to STARTUPINFO structure.
    &cPI))         // Pointer to PROCESS_INFORMATION structure.
  {
    // Theoretically possible ... but really ... how???
    // if ( ERROR_ELEVATION_REQUIRED == GetLastError() )
    MessageBoxW(
      hParent,
      L"Unable to launch " MY_EXE_NAME L"!",
      MY_COM_TITLE L" - Error",
      MB_ICONERROR | MB_OK);
  }
  else
  {
    if ( !fFilesOnCLI )
    {
      this->SendFilesToNewProcessWindow( hParent, cPI );
    }

    CloseHandle( cPI.hProcess );
    CloseHandle( cPI.hThread  );
  }

  if ( pCommandLine )
  {
    free( pCommandLine );
  }

  return S_OK;
}

//
// ****************************************************************************
//

#ifndef INITIAL_DROP_DELAY
#define INITIAL_DROP_DELAY 300
#endif

HRESULT CEditWithMyApp::SendFilesToNewProcessWindow(
  HWND hParent, PROCESS_INFORMATION &cPI )
{
  // Remember the session windows that already existed when we started.

  UINT uNumOldWindows;
  HWND aOldWindowList [MAX_EXISTING_WINDOWS];

  memcpy_s( aOldWindowList, sizeof( aOldWindowList ), this->m_aWindowList,
    ( uNumOldWindows = this->m_uNumExistingWindows ) * sizeof( HWND ) );

  // Wait for the new session to create it's main window, with a 30 second
  // timeout at 100ms per loop.
  //
  // N.B. Do NOT delay with CoWaitForMultipleHandles()!
  //      EditPad and some others will fail to work reliably.

  int iTimeout = 300;

  while ( --iTimeout )
  {
    Sleep( 100 );

    this->m_hFoundWindow = NULL;

    // Some editors create their new session in a different process from
    // the one that we've just started, so search for a new window.

    this->m_uNumExistingWindows = 0;
    EnumWindows( this->EnumWindowCallback, (LPARAM) this );

    this->m_hFoundWindow = NULL;

    for ( UINT i = 0; i < this->m_uNumExistingWindows; i++ )
    {
      this->m_hFoundWindow = this->m_aWindowList[ i ];

      for ( UINT j = 0; j < uNumOldWindows; j++ )
      {
        if ( this->m_hFoundWindow == aOldWindowList[ j ] )
        {
          this->m_hFoundWindow = NULL;
          break;
        }
      }

      if ( this->m_hFoundWindow )
      {
        break;
      }
    }

    // If we've found a new window ... is it ready for a drag-n-drop?

    if ( this->m_hFoundWindow )
    {
#ifdef USE_OLE_DODRAGDROP
      // Wait for the editor to register it's IDropTarget.
      if ( GetPropW( this->m_hFoundWindow, L"OleDropTargetInterface" ) )
#else
      // Wait for the editor's message queue to be ready.
      if ( WaitForInputIdle( cPI.hProcess, 0 ) != WAIT_TIMEOUT )
#endif
        break;
    }
  }

  if ( this->m_hFoundWindow )
  {
    // Give new window just a little bit more time to settle.
    Sleep( INITIAL_DROP_DELAY );

    this->SendFilesToOldAppInstance( hParent, this->m_hFoundWindow );

    return S_OK;
  }

  MessageBoxW(
    hParent,
    L"Unable to find new editor session!",
    MY_COM_TITLE L" - Error",
    MB_ICONERROR | MB_OK);

  return S_OK;
}

//
// ****************************************************************************
//

#ifdef MY_APP_PATH_FIXED

  // Look for it in the "Program Files" directory.

  bool CEditWithMyApp::LocateMyApp(
    wchar_t* aExecPath, DWORD nMaxChars )
  {
    ExpandEnvironmentStringsW(
      L"%ProgramFiles%\\" MY_APP_PATH_FIXED, aExecPath, nMaxChars );
    if ( PathFileExists( aExecPath ) ) return true;

  #ifdef _WIN64
    ExpandEnvironmentStringsW(
      L"%ProgramFiles(x86)%\\" MY_APP_PATH_FIXED, aExecPath, nMaxChars );
    if ( PathFileExists( aExecPath ) ) return true;
  #endif

    aExecPath[0] = L'\0';
    return false;
  }

#else

#ifdef MY_APP_PATH_KEY

  // Look for it in the "HKCR\Applications\" registry key.

  bool CEditWithMyApp::LocateMyApp(
    wchar_t* aExecPath, DWORD nMaxChars )
  {
    wchar_t aTmpPath [ MAX_PATH ];

    DWORD uKeyTyp;
    DWORD uKeyLen;

    const wchar_t * pAppPathKey = MY_APP_PATH_KEY;

    uKeyLen = sizeof( aTmpPath ) - 2;

    if ( ( SHGetValueW( HKEY_CLASSES_ROOT, pAppPathKey,
      NULL, &uKeyTyp, aTmpPath, &uKeyLen ) == ERROR_SUCCESS )
      && ( uKeyTyp == REG_SZ || uKeyTyp == REG_EXPAND_SZ ) )
    {
      if ( uKeyLen )
      {
        aTmpPath[ ( uKeyLen / sizeof(wchar_t) ) ] = L'\0';

        wchar_t * pTmpPath = wcschr( aTmpPath+1, L'\"' );

        if ( NULL != pTmpPath )
        {
          pTmpPath[0] = L'\0';

          ExpandEnvironmentStringsW( aTmpPath+1, aExecPath, nMaxChars );
  
          if ( PathFileExists( aExecPath ) ) return true;
        }
      }
    }

    aExecPath[0] = L'\0';
    return false;
  }

#else // !MY_APP_PATH_KEY

  // Look for it in the "SOFTWARE\Microsoft\Windows\CurrentVersion\App Paths\"
  // registry key ... there are 4 versions of it!!!

  bool CEditWithMyApp::LocateMyApp(
    wchar_t* aExecPath, DWORD nMaxChars )
  {
    wchar_t aTmpPath [ MAX_PATH ];

    DWORD uKeyTyp;
    DWORD uKeyLen;

    const wchar_t * pAppPathKey = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\" MY_EXE_NAME;

  #ifdef _WIN64
    for (UINT i = 0; i < 2; i++)
    {
  #endif
      for (UINT j = 0; j < 2; j++)
      {
        uKeyLen = sizeof( aTmpPath ) - 2;

        if ( ( SHGetValueW(
               (j == 0) ? HKEY_CURRENT_USER : HKEY_LOCAL_MACHINE,
               pAppPathKey, NULL, &uKeyTyp, aTmpPath, &uKeyLen ) != ERROR_SUCCESS )
          || ( uKeyTyp != REG_SZ && uKeyTyp != REG_EXPAND_SZ ) )
        {
          uKeyLen = 0;
        }

        if ( uKeyLen ) break;
      }
  #ifdef _WIN64
      if ( uKeyLen ) break;

      pAppPathKey = L"SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\App Paths\\" MY_EXE_NAME;
    }
  #endif

    if ( uKeyLen )
    {
      aTmpPath[ ( uKeyLen / sizeof(wchar_t) ) ] = L'\0';

      ExpandEnvironmentStringsW( aTmpPath, aExecPath, nMaxChars );

      if ( PathFileExists( aExecPath ) ) return true;
    }

    aExecPath[0] = L'\0';
    return false;
  }

#endif // MY_APP_PATH_KEY

#endif // MY_APP_PATH_UNREGISTERED

//
// ****************************************************************************
//

#ifndef USE_OLE_DODRAGDROP

HRESULT CEditWithMyApp::SendFilesToOldAppInstance(
  HWND hParent, HWND hTopWindow )
{
  // Workspace needed = String length + NULL file terminators + a NULL list terminator.
  UINT uBufNeed =
    this->m_uNumCharsInList + (this->m_uNumFilesInList * 1) + 1;

  HGLOBAL hGlobal = GlobalAlloc( GHND, sizeof(DROPFILES) + uBufNeed * sizeof(wchar_t) );

  if ( NULL == hGlobal )
  {
    #ifdef _DEBUG
      MessageBoxW(
        hParent,
        L"Unable to allocate global memory!",
        MY_COM_TITLE L" - Error",
        MB_ICONERROR | MB_OK);
    #endif
    return FALSE;
  }

  HWND hTargetWin = hTopWindow;

  WINDOWINFO cWindowInfo;
  cWindowInfo.cbSize = sizeof( WINDOWINFO );

#if AUTO_DETECT_OLE_DODRAGDROP
  if ( !GetWindowInfo( hTargetWin, &cWindowInfo ) )
  {
    GlobalFree( hGlobal );
    return FALSE;
  }

  // Can this top-level window accept old-style WM_DROPFILES messages?
  if ( 0 == ( cWindowInfo.dwExStyle & WS_EX_ACCEPTFILES ) )
  {
    // Does it take OLE messages instead?
    if ( NULL != (IUnknown *) GetPropW( hTargetWin, L"OleDropTargetInterface" ) )
    {
      // The window has registered an IDropTarget for OLE.
      // T.B.D.
    }
  }
#endif

#ifdef DROP_ON_CHILD_WINDOW

  this->m_hFoundWindow = NULL;

  EnumChildWindows( hTargetWin, this->EnumChildWindowCallbackFindClass, (LPARAM) this );

  if ( NULL == this->m_hFoundWindow )
    return FALSE;

  if ( !GetWindowInfo( this->m_hFoundWindow, &cWindowInfo ) )
    return FALSE;

#else

  if ( !GetWindowInfo( hTargetWin, &cWindowInfo ) )
    return FALSE;

#endif

  DROPFILES * pDropFiles = static_cast<DROPFILES *> ( GlobalLock( hGlobal ) );

  pDropFiles->pFiles = sizeof(DROPFILES);
  pDropFiles->pt.x   =
    ( cWindowInfo.rcClient.left + cWindowInfo.rcClient.right ) /2;
  pDropFiles->pt.y   =
    ( cWindowInfo.rcClient.top + cWindowInfo.rcClient.bottom ) /2;
  pDropFiles->fNC    = FALSE;
  pDropFiles->fWide  = TRUE;

  wchar_t * pBufData = reinterpret_cast<wchar_t *> (pDropFiles + 1);
  UINT      uBufLeft = uBufNeed;
  BOOL      bBuiltOK = TRUE;

  // Make a copy of the list of files that we've been passed.
  for ( UINT i = 0; i < this->m_uNumFilesInList; i++ )
  {
    UINT uCurSize = DragQueryFileW( this->m_hDrop, i, pBufData, uBufLeft );
    if (uCurSize == 0)
    {
      bBuiltOK = FALSE;
      break;
    }
    pBufData += uCurSize;
    uBufLeft -= uCurSize;
    pBufData[0] = 0;
    pBufData += 1;
    uBufLeft -= 1;
  }
  // Write a final terminating NULL;
  pBufData[0] = 0;
  pBufData += 1;
  uBufLeft -= 1;

  GlobalUnlock( hGlobal );

  if (!bBuiltOK || uBufLeft != 0)
  {
    #ifdef _DEBUG
        MessageBoxW(
          hParent,
          L"Failed to create DROPFILES structure!",
          MY_COM_TITLE L" - Error",
          MB_ICONERROR | MB_OK);
    #endif
    GlobalFree( hGlobal );
    return FALSE;
  }

  // Bring the existing target window to the foreground.
  if ( IsIconic( hTopWindow ) != 0 )
  {
    ShowWindow( hTopWindow, SW_RESTORE );
  }
  else
  {
    ShowWindow( hTopWindow, SW_SHOW );
  }
  SetForegroundWindow( hTopWindow );

  // Send the target window the WM_DROPFILES message that we've created.
  if ( PostMessageW( hTargetWin, WM_DROPFILES, (WPARAM) hGlobal, 0 ) == 0 )
  {
    // Win7 is returning 0 instead of ERROR_ACCESS_DENIED!!!
    DWORD dwError = GetLastError();
    if ( dwError == ERROR_ACCESS_DENIED || dwError == 0 )
    {
      MessageBoxW(
        hParent,
        L"Cannot send files to an ADMINISTRATOR session!",
        MY_COM_TITLE L" - Error",
        MB_ICONERROR | MB_OK);
    }
    GlobalFree( hGlobal );
  }

  return S_OK;
}

#endif

//
// ****************************************************************************
//

#ifdef DROP_ON_CHILD_WINDOW

BOOL CALLBACK CEditWithMyApp::EnumChildWindowCallbackFindClass (
  HWND hWindow, LPARAM lParam )
{
  if ( IsWindowVisible( hWindow ) && IsWindowEnabled( hWindow ) )
  {
    wchar_t aTempString [ 64 ];

    if ( GetClassNameW( hWindow, aTempString, ARRAYSIZE( aTempString ) ) )
    {
      if ( wcscmp( aTempString, DROP_ON_CHILD_WINDOW ) == 0 )
      {
        CEditWithMyApp * pThis = reinterpret_cast<CEditWithMyApp *> (lParam);
        pThis->m_hFoundWindow = hWindow;

        return FALSE;
      }
    }
  }
  return TRUE;
}

#endif

//
// ****************************************************************************
//

#ifdef USE_OLE_DODRAGDROP

class CMyDropSource : public IDropSource
{
public:    
  CMyDropSource();

  // IUnknown methods.
  STDMETHOD( QueryInterface ) ( REFIID, LPVOID * );
  STDMETHOD_( ULONG, AddRef ) ( void );
  STDMETHOD_( ULONG, Release ) ( void );

  // IDropSource methods.
  STDMETHOD( QueryContinueDrag ) ( BOOL, DWORD );
  STDMETHOD( GiveFeedback ) ( DWORD );

  ULONG m_uRefCount;

  int m_iAllowedCount;
  int m_iTimeoutCount;
};  

CMyDropSource::CMyDropSource()
{
  this->m_uRefCount     = 1;
  this->m_iAllowedCount = 0;
  this->m_iTimeoutCount = 0;
}

STDMETHODIMP CMyDropSource::QueryInterface( REFIID riid, LPVOID *ppv )
{
  if ( IsEqualIID( riid, IID_IUnknown )
    || IsEqualIID( riid, IID_IDropSource ) )
  {
    *ppv = this;
    return S_OK;
  }
  *ppv = NULL;
  return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) CMyDropSource::AddRef( void )
{
  return InterlockedIncrement( (LPLONG) &this->m_uRefCount );
}

STDMETHODIMP_(ULONG) CMyDropSource::Release( void )
{
  if ( InterlockedDecrement( (LPLONG) &this->m_uRefCount ) )
  {
    return this->m_uRefCount;
  }
  delete this;
  return 0L;
}  

STDMETHODIMP CMyDropSource::QueryContinueDrag( BOOL fEscapePressed, DWORD grfKeyState )
{  
  if ( this->m_iAllowedCount > 0 )
    return DRAGDROP_S_DROP;
  if ( this->m_iTimeoutCount > 9 )
    return DRAGDROP_S_CANCEL;
  return S_OK;
}

STDMETHODIMP CMyDropSource::GiveFeedback( DWORD dwEffect )
{
  if ( dwEffect & DROPEFFECT_COPY )
    this->m_iAllowedCount++;
  else
    this->m_iTimeoutCount++;
  return DRAGDROP_S_USEDEFAULTCURSORS;
}

#endif

//
// ****************************************************************************
//

#ifdef USE_OLE_DODRAGDROP

HRESULT CEditWithMyApp::SendFilesToOldAppInstance(
  HWND hParent, HWND hTopWindow )
{
  // Bring the existing top window to the foreground.
  if ( IsIconic( hTopWindow ) != 0 )
  {
    ShowWindow( hTopWindow, SW_RESTORE );
  }
  else
  {
    ShowWindow( hTopWindow, SW_SHOW );
  }
  SetForegroundWindow( hTopWindow );

  Sleep( 50 ); // N.B. Do NOT CoWaitForMultipleHandles()!

  HWND hTargetWin = hTopWindow;

  WINDOWINFO cWindowInfo;
  cWindowInfo.cbSize = sizeof( WINDOWINFO );

  if ( !GetWindowInfo( hTargetWin, &cWindowInfo ) )
    return FALSE;

  // Move the cursor to a point on our app's window where we know that it
  // will accept the drag-and-drop operation.
  SetCursorPos(
    ( cWindowInfo.rcClient.left + cWindowInfo.rcClient.right ) /2,
    ( cWindowInfo.rcClient.top + cWindowInfo.rcClient.bottom ) /2 );

  if ( FAILED( OleInitialize( NULL ) ) ) return FALSE;

  LPDROPSOURCE pMyDropSource = (LPDROPSOURCE) new CMyDropSource;

  DWORD dwEffect = DROPEFFECT_COPY;

  DoDragDrop( this->m_pDataObject, pMyDropSource, DROPEFFECT_COPY, &dwEffect );

  pMyDropSource->Release();

  OleUninitialize();

  return S_OK;
}

#endif

//
// ****************************************************************************
//



