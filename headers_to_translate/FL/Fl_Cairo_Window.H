//
// Main header file for the Fast Light Tool Kit (FLTK).
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

/* \file
    Fl_Cairo_Window Handling transparently a FLTK window incorporating a cairo draw callback.
*/

#ifndef FL_CAIRO_WINDOW_H
# define FL_CAIRO_WINDOW_H
# ifdef FLTK_HAVE_CAIRO

// Cairo is currently supported for the following platforms:
// Win32, Apple Quartz, X11
#  include <FL/Fl.H>
#  include <FL/Fl_Double_Window.H>

/**
   \addtogroup group_cairo
   @{
*/

/**
   This defines a FLTK window with cairo support.

   This class overloads the virtual draw() method for you,
   so that the only thing you have to do is to provide your cairo code.
   All cairo context handling is achieved transparently.

   \note You can alternatively define your custom cairo FLTK window,
   and thus at least override the draw() method to provide custom cairo
   support. In this case you will probably use Fl::cairo_make_current(Fl_Window*)
   to attach a context to your window. You should do it only when your window is
   the current window. \see Fl_Window::current()
*/
class Fl_Cairo_Window : public Fl_Double_Window {

public:
  Fl_Cairo_Window(int W, int H, const char* L = 0)
    : Fl_Double_Window(W, H, L), draw_cb_(0) {}
  Fl_Cairo_Window(int X, int Y, int W, int H, const char* L = 0)
    : Fl_Double_Window(X, Y, W, H, L), draw_cb_(0) {}

protected:
  /** Overloaded to provide cairo callback support */
  void draw() {
    Fl_Double_Window::draw();

    if (draw_cb_) { // call the Cairo draw callback
      // manual method ? if yes explicitly get a cairo_context here
      if (!Fl::cairo_autolink_context())
        Fl::cairo_make_current(this);

      draw_cb_(this, Fl::cairo_cc());
      // flush cairo drawings: necessary at least for Windows
      cairo_surface_t* s = cairo_get_target(Fl::cairo_cc());
      cairo_surface_flush(s);
    }
  }

public:
  /** This defines the cairo draw callback prototype that you must further */
  typedef void (*cairo_draw_cb)(Fl_Cairo_Window* self, cairo_t* def);
  /**
    You must provide a draw callback which will implement your cairo rendering.
    This method will permit you to set your cairo callback to \p cb.
  */
  void set_draw_cb(cairo_draw_cb cb) { draw_cb_ = cb; }
private:
  cairo_draw_cb draw_cb_;
};


/** @} */

# endif // FLTK_HAVE_CAIRO
#endif // FL_CAIRO_WINDOW_H
