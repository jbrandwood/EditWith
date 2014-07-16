// ****************************************************************************
// ****************************************************************************
//
// EditWithMyApp.h : Declaration of CEditWithMyApp shell extension object
//
//  Copyright John Brandwood 2014.
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//    http://www.boost.org/LICENSE_1_0.txt)
//

#if _MSC_VER > 1000
#pragma once
#endif

#include "config.h"

#include <shlobj.h>
#include <shellapi.h>

#define MAX_EXISTING_WINDOWS 16

#define PATH_BUFFER_CHARS 4096
#define OPTS_BUFFER_CHARS 128

#define MAX_FILE_DISPLAY_WIDTH 64

//
//
//

extern bool g_fRunningOnXP;

extern const CLSID CLSID_MyExtensionClass;
extern const wchar_t * szMyExtensionTitle;

//
// ****************************************************************************
//

class CEditWithMyApp :
  public IShellExtInit,
  public IContextMenu3
{
public:
   CEditWithMyApp();
  ~CEditWithMyApp();

public:
  // IUnknown members.
  STDMETHOD( QueryInterface ) ( REFIID, LPVOID * );
  STDMETHOD_( ULONG, AddRef ) ( void );
  STDMETHOD_( ULONG, Release ) ( void );

  // IShellExtInit members.
  STDMETHOD( Initialize ) ( PCIDLIST_ABSOLUTE, IDataObject *, HKEY );

  // IContextMenu members.
  STDMETHOD( GetCommandString ) ( UINT_PTR, UINT, UINT *, LPSTR, UINT );
  STDMETHOD( InvokeCommand ) ( LPCMINVOKECOMMANDINFO );
  STDMETHOD( QueryContextMenu ) ( HMENU, UINT, UINT, UINT, UINT );

  // IContextMenu2 members. (Only needed for rendering on WinXP)
  STDMETHOD( HandleMenuMsg ) ( UINT, WPARAM, LPARAM );

  // IContextMenu3 members. (Only needed for rendering on WinXP)
  STDMETHOD( HandleMenuMsg2 ) ( UINT, WPARAM, LPARAM, LRESULT * );

protected:
  ULONG        m_uRefCount;
  HINSTANCE    m_hDllInstance;

  BOOL OnCreation( void );
  void OnDestruction( void );

  HBITMAP GetMenuIcon( void );

  // Separate the DLL & COM code from the actual functional implementation.

  LPDATAOBJECT        m_pDataObject;
  STGMEDIUM           m_cStgMedium;
  HDROP               m_hDrop;
  UINT                m_uNumFilesInList;
  UINT                m_uNumCharsInList;

  wchar_t             m_aAppOptions [OPTS_BUFFER_CHARS];
  bool                m_fShowExisting;
  bool                m_fShowDiff;
  bool                m_fShowHex;

  UINT                m_uNumExistingWindows;
  HWND                m_aWindowList [MAX_EXISTING_WINDOWS];

  HWND                m_hFoundWindow;

  void FreeDataObject ( void );
  bool GetFileList    ( void );

  char MenuCmdToAction( UINT uMenuCmd, UINT * pMenuCmd );

  HRESULT SendFilesToNewAppInstance(
    HWND hParent, const wchar_t * szExtraOpts );

  HRESULT SendFilesToNewProcessWindow(
    HWND hParent, PROCESS_INFORMATION &cPI );

  HRESULT SendFilesToOldAppInstance(
    HWND hParent, HWND hTarget );

  bool LocateMyApp( wchar_t *aExecPath, DWORD nMaxChars );

  void GetMyOptions( void );

  static BOOL CALLBACK EnumWindowCallback(
    HWND hWindow, LPARAM lParam );

#ifdef DROP_ON_CHILD_WINDOW
  static BOOL CALLBACK EnumChildWindowCallbackFindClass(
    HWND hWindow, LPARAM lParam );
#endif

  static HBITMAP s_hIconBitmap;
  static int     s_iIconW;
  static int     s_iIconH;

public:

  // To let the DLL free up our cached resources.
  static void FreeStaticResources( void );
};

//
// ****************************************************************************
//
