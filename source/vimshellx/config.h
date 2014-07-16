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
// Class ID = {95FF2471-55D2-4B22-A352-B1299F199C29}
//

#define MY_COM_CLASS {0x95FF2471,0x55D2,0x4B22,{0xA3,0x52,0xB1,0x29,0x9F,0x19,0x9C,0x29}}
#define MY_COM_TITLE L"EditWithVim"

#define MY_WIN_CLASS_STR  L"Vim"

// Vim does not register itself in "App Paths" so look for it in the
// older "Applications" key instead.

#define MY_APP_PATH_KEY L"Applications\\gvim.exe\\shell\\edit\\command"

#define MY_EXE_NAME L"gvim.exe"

#define MY_EXE_OPTS L"--literal"

#define MY_DIFF_OPTS L" -d"
#define MAXIMUM_DIFF 4

//
// ****************************************************************************
//
