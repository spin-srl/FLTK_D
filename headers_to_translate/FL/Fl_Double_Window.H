//
// Double-buffered window header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2010 by Bill Spitzak and others.
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

/* \file
   Fl_Double_Window widget . */

#ifndef Fl_Double_Window_H
#define Fl_Double_Window_H

#include "Fl_Window.H"

/**
  The Fl_Double_Window provides a double-buffered window.
  If possible this will use the X double buffering extension (Xdbe).  If
  not, it will draw the window data into an off-screen pixmap, and then
  copy it to the on-screen window.
  <P>It is highly recommended that you put the following code before the
  first show() of <I>any</I> window in your program: </P>
  \code
  Fl::visual(FL_DOUBLE|FL_INDEX)
  \endcode
  This makes sure you can use Xdbe on servers where double buffering
  does not exist for every visual.
*/
class Fl_Double_Window : public Fl_Window
{
public:
  /**
    Return non-null if this is an Fl_Overlay_Window object.
  */
  virtual Fl_Double_Window* as_double_window() {return this; }
  void show() override;
  // void show(int a, char** b) {Fl_Window::show(a, b);}
  void resize(int, int, int, int);
  void hide() override;
  void flush();
  ~Fl_Double_Window();

  /**
   Creates a new Fl_Double_Window widget using the given
   position, size, and label (title) string.
  */
  Fl_Double_Window(int W, int H, const char* l = 0);

  /**
   See  Fl_Double_Window::Fl_Double_Window(int w, int h, const char *label = 0)
  */
  Fl_Double_Window(int X, int Y, int W, int H, const char* l = 0);
};

#endif
