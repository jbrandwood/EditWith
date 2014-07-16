// ****************************************************************************
// ****************************************************************************
//
// ResourceFuncs.h : Various utility funcs for accessing resource data.
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

#include <gdiplus.h>

//
// ****************************************************************************
//

// Fast access to internationalized string resources.

extern BOOL SetMyResourceStringModule (
  HMODULE hModule );
extern BOOL SetMyResourceStringLanguage (
  WORD wLanguage );
extern UINT GetMyResourceString (
  UINT uResourceID, wchar_t * pBufPtr, UINT uBufLen );

// Load a GDI+ bitmap from a resource (GDI+ must already be initialized).

extern Gdiplus::Bitmap * LoadMyBitmapResource (
  HINSTANCE hInstance, LPCTSTR lpBitmapName );

// Load an Exe's icon as a GDI+ bitmap (GDI+ must already be initialized).
extern Gdiplus::Bitmap * LoadMyAppIconAsBitmap (
  LPCTSTR pExeFile, BYTE * pPixelData, UINT uIconSz );

//
// ****************************************************************************
//
