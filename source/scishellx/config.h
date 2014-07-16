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
// Class ID = {8E4CFFE7-33C1-41B3-A52F-09842B8B8C9A}
//

#define MY_COM_CLASS {0x8E4CFFE7,0x33C1,0x41B3,{0xA5,0x2F,0x09,0x84,0x2B,0x8B,0x8C,0x9A}}
#define MY_COM_TITLE L"EditWithSciTE"

#define MY_WIN_CLASS_STR L"SciTEWindow"

// SciTE does not install ... and so does not register itself like Microsoft
// recommends. So try looking for it in the "Program Files" directory under
// it's default folder name.

#define MY_APP_PATH_FIXED L"wscite\\SciTE.exe"

#define MY_EXE_NAME L"SciTE.exe"
#define MY_EXE_OPTS L""

#define GET_ICON_FROM_MY_EXE

//
// ****************************************************************************
//
