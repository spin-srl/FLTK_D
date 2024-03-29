//
// Windows system header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2018 by Bill Spitzak and others.
//
// This library is free software. Distribution and use rights are outlined in
// the file "COPYING" which should have been included with this file.  If this
// file is missing or damaged, see the license at:
//
//     https://www.fltk.org/COPYING.php
//
// Please see the following page on how to report bugs and issues:
//
//     https://www.fltk.org/bugs.php
//

// Do not directly include this file, instead use <FL/platform.H>.  It will
// include this file if _WIN32 is defined.  This is to encourage
// portability of even the system-specific code...

#ifndef FL_DOXYGEN
#ifndef FL_PLATFORM_H
#  error "Never use <FL/win32.H> directly; include <FL/platform.H> instead."
#endif // !FL_PLATFORM_H

#include <windows.h>
typedef HWND Window;

// this part is included only when compiling the FLTK library or if requested explicitly
#if defined(FL_LIBRARY) || defined(FL_INTERNALS)

// In some of the distributions, the gcc header files are missing some stuff:
#ifndef LPMINMAXINFO
#define LPMINMAXINFO MINMAXINFO*
#endif
#ifndef VK_LWIN
#define VK_LWIN 0x5B
#define VK_RWIN 0x5C
#define VK_APPS 0x5D
#endif

extern UINT fl_wake_msg;
extern char fl_override_redirect; // hack into Fl_Window::make_xid()
extern int fl_background_pixel;  // hack into Fl_Window::make_xid()
extern HPALETTE fl_palette; // non-zero only on 8-bit displays!
extern void fl_release_dc(HWND w, HDC dc);
extern void fl_save_dc(HWND w, HDC dc);

#endif // FL_LIBRARY || FL_INTERNALS

// most recent fl_color() or fl_rgbcolor() points at one of these:
extern struct Fl_XMap {
	COLORREF rgb; // this should be the type the RGB() macro returns
	HPEN pen;     // pen, 0 if none created yet
	int brush;    // ref to solid brush, 0 if none created yet
	int pwidth;   // the width of the pen, if present
}* fl_current_xmap;
inline COLORREF fl_RGB() {return fl_current_xmap->rgb;}
inline HPEN fl_pen() {return fl_current_xmap->pen;}
HBRUSH fl_brush(); // allocates a brush if necessary
HBRUSH fl_brush_action(int); // now does the real work

extern HINSTANCE fl_display;
extern HDC fl_gc;
extern MSG fl_msg;
extern HDC fl_GetDC(Window);
extern HDC fl_makeDC(HBITMAP);

#endif // FL_DOXYGEN
