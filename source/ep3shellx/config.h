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
// Class ID = {3B8444F9-111B-4921-81D7-213276E36430}
//

#define MY_COM_CLASS {0x3B8444F9,0x111B,0x4921,{0x81,0xD7,0x21,0x32,0x76,0xE3,0x64,0x30}}
#define MY_COM_TITLE L"EditWithEP3"

// N.B. EditPlus uses a dynamic MFC window class name that can (and does) change.
// See MSDN article "TN070: MFC Window Class Names".
#define MY_WIN_CLASS_STR L"Afx:"

// The actual numbers can change with every new window, so limit the test length.
#define MY_WIN_CLASS_LEN 4

// Distinguish EditPlus from UltraEdit/UEStudio
#define MY_WIN_TITLE_STR L"EditPlus"

// Final test to ensure that we've got the right existing session window.
#define MY_EXE_IMAGE_CHK

#define MY_EXE_NAME L"editplus.exe"
#define MY_EXE_OPTS L""

//
// ****************************************************************************
//
