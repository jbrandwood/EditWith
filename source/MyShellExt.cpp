// ****************************************************************************
// ****************************************************************************
//
// MyShellExt.cpp : Implementation of simple apartment-threaded COM DLL
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

//
// Global variables
//

bool      g_fRunningOnXP     = false; // Are we running Windows XP or lower?

LONG      g_lThisDllRefCount = 0;     // Reference count of this DLL.
HINSTANCE g_hThisDllInstance = NULL;  // Handle to this DLL itself.

static inline void IncThisDLLRefCount() { InterlockedIncrement( &g_lThisDllRefCount ); }
static inline void DecThisDLLRefCount() { InterlockedDecrement( &g_lThisDllRefCount ); }

//
// ****************************************************************************
// ****************************************************************************
//

CEditWithMyApp::CEditWithMyApp()
{
  IncThisDLLRefCount();

  this->m_uRefCount    = 0;
  this->m_hDllInstance = g_hThisDllInstance;

  // Separate the DLL & COM code here from the actual functional implementation.
  this->OnCreation();
}

//
// ****************************************************************************
//

CEditWithMyApp::~CEditWithMyApp()
{
  // Separate the DLL & COM code here from the actual functional implementation.
  this->OnDestruction();

  DecThisDLLRefCount();
}

//
// ****************************************************************************
//

STDMETHODIMP CEditWithMyApp::QueryInterface( REFIID riid, LPVOID *ppv )
{
  if ( NULL == ppv )
  {
    return E_POINTER;
  }
  *ppv = NULL;
  if ( IsEqualIID( riid, IID_IUnknown ) )
  {
    *ppv = static_cast<IUnknown *> ( static_cast<IShellExtInit *> (this) );
  }
  else
  if ( IsEqualIID( riid, IID_IShellExtInit ) )
  {
    *ppv = static_cast<IShellExtInit *> (this);
  }
  else
  if ( IsEqualIID( riid, IID_IContextMenu  ) ||
       IsEqualIID( riid, IID_IContextMenu2 ) ||
       IsEqualIID( riid, IID_IContextMenu3 ) )
  {
    *ppv = static_cast<IContextMenu3 *> (this);
  }
  if ( NULL == *ppv )
  {
    return E_NOINTERFACE;
  }
  this->AddRef();
  return S_OK;
}

//
// ****************************************************************************
//

STDMETHODIMP_(ULONG) CEditWithMyApp::AddRef( void )
{
  return InterlockedIncrement( (LPLONG) &this->m_uRefCount );
}

//
// ****************************************************************************
//

STDMETHODIMP_(ULONG) CEditWithMyApp::Release( void )
{
  if (InterlockedDecrement( (LPLONG) &this->m_uRefCount ))
  {
    return this->m_uRefCount;
  }
  delete this;
  return 0L;
}

//
// ****************************************************************************
// ****************************************************************************
//

class CMyClassFactory : public IClassFactory
{
protected:
  ULONG  m_uRefCount;

public:
   CMyClassFactory();
  ~CMyClassFactory();

  // IUnknown members.
  STDMETHOD( QueryInterface ) ( REFIID, LPVOID * );
  STDMETHOD_( ULONG, AddRef ) ( void );
  STDMETHOD_( ULONG, Release ) ( void );

  // IClassFactory members.
  STDMETHOD( CreateInstance ) ( LPUNKNOWN, REFIID, LPVOID * );
  STDMETHOD( LockServer ) ( BOOL );
};

//
// ****************************************************************************
//

CMyClassFactory::CMyClassFactory()
{
  IncThisDLLRefCount();
  this->m_uRefCount = 0L;
}

//
// ****************************************************************************
//

CMyClassFactory::~CMyClassFactory()
{
  DecThisDLLRefCount();
}

//
// ****************************************************************************
//

STDMETHODIMP CMyClassFactory::QueryInterface(
 REFIID riid, LPVOID *ppv )
{
  if ( NULL == ppv )
  {
    return E_POINTER;
  }
  *ppv = NULL;
  if ( IsEqualIID( riid, IID_IUnknown ) )
  {
    *ppv = static_cast<IUnknown *> (this);
  }
  else
  if ( IsEqualIID( riid, IID_IClassFactory ) )
  {
    *ppv = static_cast<IClassFactory *> (this);
  }
  if ( NULL == *ppv )
  {
    return E_NOINTERFACE;
  }
  this->AddRef();
  return S_OK;
}

//
// ****************************************************************************
//

STDMETHODIMP_(ULONG) CMyClassFactory::AddRef( void )
{
  return InterlockedIncrement( (LPLONG) &this->m_uRefCount );
}

//
// ****************************************************************************
//

STDMETHODIMP_(ULONG) CMyClassFactory::Release( void )
{
  if (InterlockedDecrement( (LPLONG) &this->m_uRefCount ))
  {
    return this->m_uRefCount;
  }
  delete this;
  return 0L;
}

//
// ****************************************************************************
//

STDMETHODIMP CMyClassFactory::CreateInstance(
  LPUNKNOWN pUnkOuter, REFIID riid, LPVOID *ppv)
{
  if ( NULL == ppv )
  {
    return E_POINTER;
  }
  *ppv = NULL;
  if ( pUnkOuter )
  {
    return CLASS_E_NOAGGREGATION;
  }
  CEditWithMyApp * pClassObject = new CEditWithMyApp;
  if ( NULL == pClassObject )
  {
    return E_OUTOFMEMORY;
  }
  pClassObject->AddRef();
  HRESULT hResult = pClassObject->QueryInterface ( riid, ppv );
  pClassObject->Release();
  return hResult;
}

//
// ****************************************************************************
//

STDMETHODIMP CMyClassFactory::LockServer( BOOL fLock )
{
  if ( fLock )
    { IncThisDLLRefCount(); }
  else
    { DecThisDLLRefCount(); }
  return S_OK;
}

//
// ****************************************************************************
// ****************************************************************************
//

extern "C" BOOL WINAPI DllMain ( HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved )
{
#ifdef _DEBUG 
// MessageBox( 0, L"Entering DllMain() ...\n\nTime to attach the debugger!", MY_COM_TITLE L" - Information", 0 );
#endif

  switch (dwReason)
  {
    case DLL_PROCESS_ATTACH:
    {
      g_hThisDllInstance = hInstance;

      DisableThreadLibraryCalls( hInstance );

      g_fRunningOnXP = ( ( LOBYTE( LOWORD( GetVersion() ))) < 6 );

      SetMyResourceStringModule( hInstance );
      break;
    }

    case DLL_PROCESS_DETACH:
    {
      if ( NULL == lpReserved )
      {
        CEditWithMyApp::FreeStaticResources();
      }
      break;
    }
  }
  return TRUE;
}

//
// ****************************************************************************
//

STDAPI DllCanUnloadNow (void)
{
  return ( (g_lThisDllRefCount == 0) ? S_OK : S_FALSE );
}

//
// ****************************************************************************
//

STDAPI DllGetClassObject ( REFCLSID rclsid, REFIID riid, LPVOID* ppv )
{
  if ( NULL == ppv )
  {
    return E_POINTER;
  }
  *ppv = NULL;
  if ( !IsEqualCLSID( rclsid, CLSID_MyExtensionClass ) )
  {
    return CLASS_E_CLASSNOTAVAILABLE;
  }
  CMyClassFactory * pClassFactory = new CMyClassFactory;
  if ( NULL == pClassFactory )
  {
    return E_OUTOFMEMORY;
  }
  pClassFactory->AddRef();
  HRESULT hResult = pClassFactory->QueryInterface( riid, ppv );
  pClassFactory->Release();
  return hResult;
}

//
// ****************************************************************************
//
// These are the registry entries required for a Context Menu shell extension.
//
// If
//   CLSID_MyExtensionClass = {8A854BCF-E3A3-4173-A1EB-D2FD9BE285F0}
// and
//   szMyExtensionTitle = L"EditWithZeus"
// then
//
// HKCU
//  NoRemove Software
//   NoRemove Classes
//    NoRemove CLSID
//     ForceRemove {8A854BCF-E3A3-4173-A1EB-D2FD9BE285F0} = s 'EditWithZeus'
//      InprocServer32 = s 'C:\Complete\Path\To\This.dll'
//       val ThreadingModel = s 'Apartment'
//    NoRemove *
//     NoRemove shellex
//      NoRemove ContextMenuHandlers
//       ForceRemove EditWithZeus = s '{8A854BCF-E3A3-4173-A1EB-D2FD9BE285F0}'
//
// HKCU
//  NoRemove Software
//   NoRemove Microsoft
//    NoRemove Windows
//     NoRemove CurrentVersion
//      NoRemove Shell Extensions
//       NoRemove Approved
//        ForceRemove {8A854BCF-E3A3-4173-A1EB-D2FD9BE285F0} = s 'EditWithZeus'

static BOOL flInstallForAllUsers = FALSE;

static wchar_t szAppCommandParameters [OPTS_BUFFER_CHARS] = MY_EXE_OPTS;

typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);

STDAPI DllRegisterServer ( void )
{
  HKEY hRootKey = (flInstallForAllUsers) ? HKEY_LOCAL_MACHINE : HKEY_CURRENT_USER;
  LONG lResult;
  HKEY hKey;
  wchar_t szCLSID  [ 40 ];
  wchar_t szModule [ MAX_PATH ];
  wchar_t szSubKey [ MAX_PATH ];

#ifndef _WIN64
  // Don't register the 32-bit DLL by accident on 64-bit Windows.
  LPFN_ISWOW64PROCESS fnIsWow64Process = (LPFN_ISWOW64PROCESS)
    GetProcAddress( GetModuleHandleA( "kernel32" ), "IsWow64Process" );
  if ( NULL != fnIsWow64Process )
  {
    BOOL bIsWow64;
    if ( fnIsWow64Process( GetCurrentProcess(), &bIsWow64 ) && bIsWow64 )
    {
      MessageBoxA(
        NULL,
        "You are attempting to register a 32-bit Explorer\n"
        "Shell Extension on a 64-bit version of Windows.\n\n"
        "Registration cancelled.\n",
        "Shell Extension - Error",
        MB_ICONERROR | MB_OK );
      return SELFREG_E_CLASS;
    }
  }
#endif

  // Convert our extension's CLSID into string format for the registry.
  StringFromGUID2( CLSID_MyExtensionClass, szCLSID, ARRAYSIZE( szCLSID ) );

  // Get this DLL's full path and file name.
  DWORD uCount = GetModuleFileNameW( g_hThisDllInstance, szModule, ARRAYSIZE( szModule ) );
  if ( uCount == 0 || uCount == ARRAYSIZE( szModule ) )
  {
    return SELFREG_E_CLASS;
  }

  wcscpy_s( szSubKey, ARRAYSIZE( szSubKey ), L"Software\\Classes\\CLSID\\" );
  wcscat_s( szSubKey, ARRAYSIZE( szSubKey ), szCLSID );

  lResult = RegCreateKeyExW(
    hRootKey, szSubKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL);
  if ( lResult != ERROR_SUCCESS ) return SELFREG_E_CLASS;

  lResult = RegSetValueExW(
    hKey,
    NULL,
    0,
    REG_SZ,
    (LPBYTE) szMyExtensionTitle,
    (DWORD) ( wcslen( szMyExtensionTitle ) + 1) * sizeof( wchar_t ) );

  if ( lResult == ERROR_SUCCESS && !flInstallForAllUsers )
  {
    lResult = RegSetValueExW(
      hKey,
      L"AppOptions",
      0,
      REG_SZ,
      (LPBYTE) szAppCommandParameters,
      (DWORD) ( wcslen( szAppCommandParameters ) + 1 ) * sizeof( wchar_t ) );
  }
      
  RegCloseKey(hKey);
  if ( lResult != ERROR_SUCCESS ) return SELFREG_E_CLASS;

  wcscat_s( szSubKey, ARRAYSIZE( szSubKey ), L"\\InprocServer32" );

  lResult = RegCreateKeyExW(
    hRootKey, szSubKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL);
  if ( lResult != ERROR_SUCCESS ) return SELFREG_E_CLASS;

  lResult = RegSetValueExW(
    hKey,
    NULL,
    0,
    REG_SZ,
    (LPBYTE) szModule,
    (DWORD) ( wcslen( szModule ) + 1 ) * sizeof( wchar_t ) );

  if ( lResult == ERROR_SUCCESS )
  {
    lResult = RegSetValueExW(
      hKey,
      L"ThreadingModel",
      0,
      REG_SZ,
      (LPBYTE) L"Apartment",
      (DWORD) ( wcslen( L"Apartment" ) + 1 ) * sizeof( wchar_t ) );
  }
      
  RegCloseKey(hKey);
  if ( lResult != ERROR_SUCCESS ) return SELFREG_E_CLASS;

  wcscpy_s( szSubKey, ARRAYSIZE( szSubKey ), L"Software\\Classes\\*\\ShellEx\\ContextMenuHandlers\\" );
  wcscat_s( szSubKey, ARRAYSIZE( szSubKey ), szMyExtensionTitle );

  lResult = RegCreateKeyExW(
    hRootKey, szSubKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL);
  if ( lResult != ERROR_SUCCESS ) return SELFREG_E_CLASS;

  lResult = RegSetValueExW(
    hKey,
    NULL,
    0,
    REG_SZ,
    (LPBYTE) szCLSID,
    (DWORD) ( wcslen( szCLSID ) + 1 ) * sizeof( wchar_t ) );

  RegCloseKey( hKey );
  if ( lResult != ERROR_SUCCESS ) return SELFREG_E_CLASS;

  wcscpy_s( szSubKey, ARRAYSIZE( szSubKey ), L"Software\\Microsoft\\Windows\\CurrentVersion\\Shell Extensions\\Approved" );

  lResult = RegCreateKeyExW(
    hRootKey, szSubKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL);
  if ( lResult != ERROR_SUCCESS ) return SELFREG_E_CLASS;

  lResult = RegSetValueExW(
    hKey,
    szCLSID,
    0,
    REG_SZ,
    (LPBYTE) szMyExtensionTitle,
    (DWORD) ( wcslen( szMyExtensionTitle ) + 1 ) * sizeof( wchar_t ) );

  RegCloseKey( hKey );
  if ( lResult != ERROR_SUCCESS ) return SELFREG_E_CLASS;

  // Let Windows Explorer know that we've made a change.
  SHChangeNotify( SHCNE_ASSOCCHANGED, SHCNF_IDLIST, NULL, NULL );

  return S_OK;
}

//
// ****************************************************************************
//

STDAPI DllUnregisterServer ( void )
{
  HKEY hRootKey = (flInstallForAllUsers) ? HKEY_LOCAL_MACHINE : HKEY_CURRENT_USER;
  LONG lResult;
  HKEY hKey;
  wchar_t szCLSID  [ 40 ];
  wchar_t szSubKey [ MAX_PATH ];

  // Convert our extension's CLSID into string format for the registry.
  StringFromGUID2( CLSID_MyExtensionClass, szCLSID, ARRAYSIZE( szCLSID ) );

  wcscpy_s( szSubKey, ARRAYSIZE( szSubKey ), L"Software\\Microsoft\\Windows\\CurrentVersion\\Shell Extensions\\Approved" );

  lResult = RegOpenKeyExW(
    hRootKey, szSubKey, 0, KEY_SET_VALUE, &hKey );
  if (( lResult != ERROR_SUCCESS ) && ( lResult != ERROR_FILE_NOT_FOUND )) return SELFREG_E_CLASS;

  if ( lResult == ERROR_SUCCESS )
  {
    lResult = RegDeleteValueW( hKey, szCLSID );
    RegCloseKey( hKey );
    if (( lResult != ERROR_SUCCESS ) && ( lResult != ERROR_FILE_NOT_FOUND )) return SELFREG_E_CLASS;
  }

  wcscpy_s( szSubKey, ARRAYSIZE( szSubKey ), L"Software\\Classes\\*\\ShellEx\\ContextMenuHandlers\\" );
  wcscat_s( szSubKey, ARRAYSIZE( szSubKey ), szMyExtensionTitle );

  lResult = RegDeleteKeyW( hRootKey, szSubKey );
  if (( lResult != ERROR_SUCCESS ) && ( lResult != ERROR_FILE_NOT_FOUND )) return SELFREG_E_CLASS;

  wcscpy_s( szSubKey, ARRAYSIZE( szSubKey ), L"Software\\Classes\\CLSID\\" );
  wcscat_s( szSubKey, ARRAYSIZE( szSubKey ), szCLSID );
  wcscat_s( szSubKey, ARRAYSIZE( szSubKey ), L"\\InprocServer32" );

  lResult = RegDeleteKeyW( hRootKey, szSubKey );
  if (( lResult != ERROR_SUCCESS ) && ( lResult != ERROR_FILE_NOT_FOUND )) return SELFREG_E_CLASS;

  wcscpy_s( szSubKey, ARRAYSIZE( szSubKey ), L"Software\\Classes\\CLSID\\" );
  wcscat_s( szSubKey, ARRAYSIZE( szSubKey ), szCLSID );

  lResult = RegDeleteKeyW( hRootKey, szSubKey );
  if (( lResult != ERROR_SUCCESS ) && ( lResult != ERROR_FILE_NOT_FOUND )) return SELFREG_E_CLASS;

  // Let Windows Explorer know that we've made a change.
  SHChangeNotify( SHCNE_ASSOCCHANGED, SHCNF_IDLIST, NULL, NULL );

  return S_OK;
}

//
// ****************************************************************************
//

STDAPI DllInstall ( BOOL bInstall, LPCWSTR pszCmdLine )
{
#ifdef _DEBUG 
 MessageBox( 0, L"Entering DllInstall() ...\n\nTime to attach the debugger!", MY_COM_TITLE L" - Information", 0 );
#endif

  // Accept either "everyone" or a set of options to pass to the app's .exe
  // on it's command line when opening a new instance.
  // By design, I won't allow both since it's a potential security hazard.
  if ( pszCmdLine )
  {
    if ( _wcsicmp( pszCmdLine, L"everyone" ) == 0 )
    {
      flInstallForAllUsers = TRUE;
    }
    else
    {
      if ( wcslen( pszCmdLine ) < ARRAYSIZE( szAppCommandParameters ) )
      {
        wcscpy_s( szAppCommandParameters, ARRAYSIZE( szAppCommandParameters ), pszCmdLine );
      }
    }
  }

  HRESULT hResult;
  if ( bInstall )
  {  
    hResult = DllRegisterServer();
    if ( hResult != S_OK )
    {  
      DllUnregisterServer();
    }
  }
  else
  {
    hResult = DllUnregisterServer();
  }
  return hResult;
}

//
// ****************************************************************************
//
