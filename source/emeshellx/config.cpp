// ****************************************************************************
// ****************************************************************************
//
// config.cpp : Customization for this specific shell extension
//
//  Copyright John Brandwood 2014.
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//    http://www.boost.org/LICENSE_1_0.txt)
//

//
// ****************************************************************************
//
// Simple function to choose which user-supplied icon to use and whether to
// scale it or center it.
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

#include "config.h"

int SelectMyIcon ( int iWantSz, bool &fCenterIcon, bool &fHQBScaling )
{
  fCenterIcon = false;

  if ( iWantSz < 20 )
  {
    fHQBScaling = false;
    return 16;
  }
  else
  {
    fHQBScaling = true;
    return 32;
  }
}

//
// ****************************************************************************
//
// Title string format is ... "C:\autoexec.bat - EmEditor"

#include <wchar.h>

void SplitTitleString (
  wchar_t * pString, wchar_t * &pHead, wchar_t * &pFile, wchar_t * &pTail )
{
  pHead =
  pFile = pString;
  pTail = wcsrchr( pFile, L'-' );
  if ( NULL == pTail || wcsncmp( pTail--, L"- EmEditor", 10 ) != 0 )
  {
    pTail = pFile + wcslen( pFile );
  }
}

//
// ****************************************************************************
//
