*******************************************************************************

EditWith
========

  **A collection of Windows Explorer shell extensions for text editors.**

  Copyright John Brandwood 2014.

  Distributed under the Boost Software License, Version 1.0.

  (See the accompanying file LICENSE_1_0.txt or the copy at
        http://www.boost.org/LICENSE_1_0.txt)

*******************************************************************************

About
-----

These shell extension DLLs add either 1 or 2 menu entries to the right-click
(context) menu of Windows Explorer, depending upon whether the text editor
is already running.

For example, the extension for the Notepad++ editor will add ...

  * Edit with Notepad++
  * Edit in existing Notepad++ session

The text editor's icon is also displayed with each entry to make it easy to
distinguish your text editor from other context menu entries.

The shell extensions are designed to work on 32-bit and 64-bit versions of
Windows, from Windows XP all the way up to Windows 8.1

*******************************************************************************

Text Editors
------------

Shell extension DLLs are supplied for the following text editors ...

  Text Editor               |  Extension
  :-------------------------|----------:
  EditPad Pro               |  eppshellx
  EditPlus 3                |  ep3shellx
  EmEditor                  |  emeshellx
  Komodo Edit / Komodo IDE  |    kshellx
  Multi-Edit                |   meshellx
  Notepad++                 |  nppshellx
  Programmer's Notepad 2    |  pn2shellx
  PSPad                     |  pspshellx
  SciTE                     |  scishellx
  SlickEdit                 |   seshellx
  Sublime Text 2            |  su2shellx
  Sublime Text 3            |  su3shellx
  TextPad                   |   tpshellx
  UEStudio                  |  uesshellx
  UltraEdit                 |   ueshellx
  VEDIT                     |    vshellx
  Vim                       |  vimshellx
  Zeus IDE                  |    zshellx

*******************************************************************************

Question
--------

  My editor already shows up in the right-click menu, why should I use this?

Answer
------

  Because unlike your editor's existing entry, it allows you to choose whether
  to open up the files that you've selected in either a new editor session, or
  to send the files to ANY existing editor session.

  This makes is easy to not only edit multiple files in one session, but also
  to have multiple editor sessions at once. It makes it easier to keep groups
  of files separate in different windows, and/or to efficiently use multiple
  monitors.

*******************************************************************************

Question
--------

  Does it offer any other advantages?

Answer
------

  It's smaller, lighter, faster, and can jump tall buildings in a single leap!

  But seriously ... compare simple sizes and how much of your computer's
  memory is used to provide these features.

  This shell extension takes less than 30KB of disk space, and when Windows
  Explorer runs it, it uses the same DLL resources that Explorer already uses,
  so that it doesn't take much extra computer memory (i.e. MSVCRT.DLL).

  By extreme comparison, the Notepad++ shell extension (NppShell_06.dll) takes
  218KB of disk space, and then still loads up the 637KB MSVCR90.DLL.

  That's a lot of disk space and computer resources for something that does
  less than this shell extension.

  In fairness, most other text editor's current shell extensions are a lot
  slimmer at 60KB to 110KB, and don't pull in the 637KB MSVCR90/100/110.DLL.

  Some of the other text editor's current shell extensions either don't render
  an icon, or render it at the wrong size (i.e. ignore Windows's requested
  size), or even render it incorrectly (i.e. on a pure-white background).

*******************************************************************************
