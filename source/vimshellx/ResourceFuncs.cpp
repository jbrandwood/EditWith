// ****************************************************************************
// ****************************************************************************
//
// ResourceFuncs.cpp : Various utility funcs for accessing resource data.
//
//  Copyright John Brandwood 2014.
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//    http://www.boost.org/LICENSE_1_0.txt)
//

#include "stdafx.h"
#include "resource.h"

#include "ResourceFuncs.h"

#include <wchar.h>
#include <gdiplus.h>
#include <shlwapi.h>
#include <shlobj.h>

//
// Static variables
//

// Strings are stored in the resource file in blocks of 16, and start at 100.
#define START_STRING_ID ( 100 & ~15 )

static const wchar_t * s_aMyResourceStrPtr [ FINAL_STRING_ID - START_STRING_ID ];

static HMODULE s_hMyStringModule = NULL;
static WORD s_wMyStringLanguage  = MAKELANGID( LANG_NEUTRAL, SUBLANG_NEUTRAL );
static BOOL s_bMyStringsLocated  = FALSE;

// See LoadMyBitmapResource().
typedef IStream * (WINAPI * LPFN_SHCREATEMEMSTREAM) (const BYTE *, UINT);
static LPFN_SHCREATEMEMSTREAM fnSHCreateMemStream = NULL;

//
// ****************************************************************************
//

static BOOL LocateMyResourceStrings ( void )
{
  s_bMyStringsLocated = FALSE;

  WORD  wLanguage = s_wMyStringLanguage;
  HMODULE hModule = s_hMyStringModule;
  if ( !hModule ) { return FALSE; };

  // Read all the resource strings into our buffer, block-by-block.
  const wchar_t ** pStringPtr = s_aMyResourceStrPtr;

  UINT    uResourceID = START_STRING_ID;
  while ( uResourceID < FINAL_STRING_ID )
  {
    // Locate the next block of 16 strings.
    HRSRC hRes = FindResourceEx(
      hModule,
      MAKEINTRESOURCE( RT_STRING ),
      MAKEINTRESOURCE( uResourceID / 16 + 1 ),
      wLanguage);
    if ( NULL == hRes ) break;

    HGLOBAL hMem = LoadResource( hModule, hRes );
    if ( NULL == hMem ) break;

    wchar_t * pRes = (wchar_t *) LockResource( hMem );
    if ( NULL == pRes ) break;

    for ( int i = 16; i != 0; --i )
    {
      *pStringPtr++ = pRes;
      pRes += ( *pRes + 1 );
      if ( FINAL_STRING_ID == ++uResourceID ) break;
    }
  }

  if ( FINAL_STRING_ID == uResourceID )
  {
    return s_bMyStringsLocated = TRUE;
  }

  return FALSE;
}

//
// ****************************************************************************
//

BOOL SetMyResourceStringModule (
  HMODULE hModule )
{
  if ( hModule == s_hMyStringModule ) return FALSE;
  s_hMyStringModule   = hModule;
  s_bMyStringsLocated = FALSE;
  return TRUE; // TRUE signals the module has changed.
}

//
// ****************************************************************************
//

BOOL SetMyResourceStringLanguage (
  WORD wLanguage )
{
  if ( wLanguage == s_wMyStringLanguage ) return FALSE;
  s_wMyStringLanguage = wLanguage;
  s_bMyStringsLocated = FALSE;
  return TRUE; // TRUE signals the language has changed.
}

//
// ****************************************************************************
//

UINT GetMyResourceString (
  UINT uResourceID, wchar_t * pBufPtr, UINT uBufLen )
{
  if ( s_bMyStringsLocated )
  {
    if ( ( uResourceID >= START_STRING_ID ) && ( uResourceID < FINAL_STRING_ID ) )
    {
      const wchar_t * pResStr = s_aMyResourceStrPtr [ uResourceID - START_STRING_ID ];

      if ( pResStr )
      {
        UINT uResLen = *pResStr++;
        if ( uBufLen )
        {
          if ( pBufPtr )
          {
            if ( uResLen >= uBufLen ) { uResLen = uBufLen - 1; }
            wmemcpy_s( pBufPtr, uBufLen, pResStr, uResLen );
            pBufPtr[ uResLen ] = L'\0';
            return uResLen;
          }
        }
        // Handle "uBufLen==0" just like Microsoft's LoadString().
        if ( pBufPtr )
        {
          *( reinterpret_cast<const wchar_t **> (pBufPtr) ) = pResStr;
        }
        return uResLen;
      }
    }
  }
  else
  if ( LocateMyResourceStrings() )
  {
    return GetMyResourceString( uResourceID, pBufPtr, uBufLen );
  }

  // String not found.
  if ( pBufPtr ) { *pBufPtr = L'\0'; }
  return 0;
}

//
// ****************************************************************************
//
// Read BMP\GIF\JPEG\PNG\TIFF\Exif\WMF\EMF format data from the resource.
//
// The resource MUST be declared as the RCDATA type and NOT the BITMAP type.
//
// N.B. GDI+ must already be initialized before calling this function.

Gdiplus::Bitmap * LoadMyBitmapResource (
  HINSTANCE hInstance, LPCTSTR lpBitmapName )
{
  // SHCreateMemStream() has existed since Win2000, but it wasn't declared in
  // the headers and import library until Vista.
  // To use it on WinXP, we need to explicitly get the address from the DLL.
  if ( NULL == fnSHCreateMemStream )
  {
    // Harmless call that's only here to force linking with shlwapi.dll so
    // that we can get the address of SHCreateMemStream().
    ColorAdjustLuma( 0, 0, FALSE );

    fnSHCreateMemStream = (LPFN_SHCREATEMEMSTREAM)
      GetProcAddress( GetModuleHandleA( "shlwapi.dll" ), (LPCSTR) 12 );

    if ( NULL == fnSHCreateMemStream ) return NULL;
  }

  HRSRC hBitmapResource = FindResource(
    hInstance, lpBitmapName, RT_RCDATA );

  if ( NULL == hBitmapResource ) return NULL;

  DWORD iResourceSize = SizeofResource( hInstance, hBitmapResource );

  if ( !iResourceSize ) return NULL;

  HGLOBAL hResourceData = LoadResource( hInstance, hBitmapResource );

  if ( NULL == hResourceData ) return NULL;

  const BYTE * pResourceData = (const BYTE *) LockResource( hResourceData );

  if ( NULL == pResourceData ) return NULL;

  IStream * pStream = fnSHCreateMemStream( pResourceData, iResourceSize );

  if ( NULL == pStream ) return NULL;

  Gdiplus::Bitmap * pBitmap = Gdiplus::Bitmap::FromStream( pStream );

  pStream->Release();

  if ( ( NULL == pBitmap ) || ( pBitmap->GetLastStatus() != Gdiplus::Ok ) )
  {
    delete pBitmap;
    return NULL;
  }

  return pBitmap;
}

//
// ****************************************************************************
//
// The GDI+ bitmap constructors that take an existing HICON or HBITMAP destroy
// any existing semi-transparent-alpha when the source is in 32bit ARGB format.
//
// The code below detects the alpha in a 32bit ARGB icon and uses a different
// GDI+ bitmap constructor that does preserve the alpha.
//
// If the icon is NOT in 32bit ARGB format, then fall back on the
// regular constructor which generates alpha from the icon's mask.
//
// N.B. The bitmap's pixel data is stored in the buffer that's passed in.
//
// N.B. GDI+ must already be initialized before calling this function.

Gdiplus::Bitmap * LoadMyAppIconAsBitmap (
  LPCTSTR pExeFile, BYTE * pPixelData, UINT uIconSz )
{
  HICON hIcon = NULL;
  if ( SHDefExtractIcon( pExeFile, 0, 0, &hIcon, NULL, uIconSz ) != S_OK )
  {
    return NULL;
  }

  Gdiplus::Bitmap * pBitmap = NULL;

  ICONINFO cIconInfo;
  ZeroMemory( &cIconInfo, sizeof( cIconInfo ) );

  if ( GetIconInfo( hIcon, &cIconInfo ) )
  {
    BITMAP cBitmap;
    if ( GetObject( cIconInfo.hbmColor, sizeof( cBitmap ), &cBitmap ) )
    {
      BITMAPINFO cBMI;
      ZeroMemory( &cBMI, sizeof( cBMI ) );
      cBMI.bmiHeader.biSize        = sizeof( cBMI.bmiHeader );
      cBMI.bmiHeader.biWidth       =   cBitmap.bmWidth;
      cBMI.bmiHeader.biHeight      = - cBitmap.bmHeight;
      cBMI.bmiHeader.biPlanes      = 1;
      cBMI.bmiHeader.biBitCount    = 32;
      cBMI.bmiHeader.biCompression = BI_RGB;

      if ( pPixelData )
      {
        HDC hDC = NULL;
        HDC hSC = GetDC( NULL );
        if ( hSC )
        {
          hDC = CreateCompatibleDC( hSC );
          ReleaseDC( NULL, hSC );
        }

        if ( hDC )
        {
          if ( GetDIBits(
                 hDC, cIconInfo.hbmColor,
                 0, cBitmap.bmHeight, pPixelData, &cBMI, DIB_RGB_COLORS ) )
          {
            BYTE * pPixel = pPixelData + cBitmap.bmWidth * cBitmap.bmHeight * 4;
            BYTE   bAlpha = 0;
            do
            {
              pPixel -= 4;
              bAlpha |= pPixel[3];
            } while ( pPixel > pPixelData );

            if ( bAlpha )
            {
              #if 1 // Short version that links to the existing pixel data.
              {
                pBitmap =
                  new Gdiplus::Bitmap(
                    cBitmap.bmWidth,
                    cBitmap.bmHeight,
                    cBitmap.bmWidth * 4,
                    PixelFormat32bppARGB,
                    pPixelData );
              }
              #else // Long version that copies the pixel data to a new buffer.
              {
                pBitmap =
                  new Gdiplus::Bitmap(
                    cBitmap.bmWidth,
                    cBitmap.bmHeight,
                    PixelFormat32bppARGB );

                if ( pBitmap )
                {
                  Gdiplus::BitmapData cBitmapData;
                  cBitmapData.Width       = cBitmap.bmWidth,
                  cBitmapData.Height      = cBitmap.bmHeight,
                  cBitmapData.Stride      = cBitmap.bmWidth * 4;
                  cBitmapData.PixelFormat = PixelFormat32bppARGB;
                  cBitmapData.Scan0       = pPixelData;
                  cBitmapData.Reserved    = NULL;

                  Gdiplus::Rect cRect( 0, 0, cBitmap.bmWidth, cBitmap.bmHeight );

                  pBitmap->LockBits(
                    &cRect,
                    Gdiplus::ImageLockModeWrite | Gdiplus::ImageLockModeUserInputBuf,
                    PixelFormat32bppARGB,
                    &cBitmapData );

                  pBitmap->UnlockBits( &cBitmapData );
                }
              }
              #endif
            }
          }
          DeleteDC ( hDC );
        }
      }
    }
    DeleteObject( cIconInfo.hbmMask );
    DeleteObject( cIconInfo.hbmColor );
  }

  if ( hIcon )
  {
    if ( pBitmap == NULL )
    {
      pBitmap =
        new Gdiplus::Bitmap( hIcon );
    }

    DestroyIcon( hIcon );
    hIcon = NULL;
  }

  return pBitmap;
}

//
// ****************************************************************************
//
