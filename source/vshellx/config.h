// ****************************************************************************
// ****************************************************************************
//
// config.h : Customization for this specific shell extension
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

//
// Class ID = {FE4E820B-A724-42C5-A552-0B2546DD7D1B}
//

#define MY_COM_CLASS {0xFE4E820B,0xA724,0x42C5,{0xA5,0x52,0x0B,0x25,0x46,0xDD,0x7D,0x1B}}
#define MY_COM_TITLE L"EditWithVedit"

#define MY_WIN_CLASS_STR  L"vwCFrame"

#define MY_EXE_NAME L"vpw.exe"
#define MY_EXE_OPTS L"-s0 -e"

// #define MY_DIFF_OPTS L" -n 1 -x compare.vdm"
// #define MAXIMUM_DIFF 2

//
// ****************************************************************************
//
