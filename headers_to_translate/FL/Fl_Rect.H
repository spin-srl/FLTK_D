//
// Fl_Rect header file for the Fast Light Tool Kit (FLTK).
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

#ifndef Fl_Rect_H
#define Fl_Rect_H

#include <FL/Fl_Widget.H>       // for c'tor based on Fl_Widget

/**
  Rectangle with standard FLTK coordinates (X, Y, W, H).

  This may be used internally, for overloaded widget contructors and other
  overloaded methods like fl_measure(), fl_text_extents(), fl_rect(),
  fl_rectf(), and maybe more.
*/

class Fl_Rect {

  int x_;
  int y_;
  int w_;
  int h_;

public:

  /** The default constructor creates an empty rectangle (x = y = w = h = 0). */
  Fl_Rect()
    : x_(0), y_(0), w_(0), h_(0) {}

  /** This constructor creates a rectangle with x = y = 0 and
    the given width and height. */
  Fl_Rect(int W, int H)
    : x_(0), y_(0), w_(W), h_(H) {}

  /** This constructor creates a rectangle with the given x,y coordinates
    and the given width and height. */
  Fl_Rect(int X, int Y, int W, int H)
    : x_(X), y_(Y), w_(W), h_(H) {}

  /** This constructor creates a rectangle based on a widget's position and size. */
  Fl_Rect(const Fl_Widget &widget)
    : x_(widget.x()), y_(widget.y()), w_(widget.w()), h_(widget.h()) {}

  /** This constructor creates a rectangle based on a widget's position and size. */
  Fl_Rect(const Fl_Widget* const widget)
    : x_(widget->x()), y_(widget->y()), w_(widget->w()), h_(widget->h()) {}

  int x() const { return x_; }          ///< gets the x coordinate (left edge)
  int y() const { return y_; }          ///< gets the y coordinate (top edge)
  int w() const { return w_; }          ///< gets the width
  int h() const { return h_; }          ///< gets the height

  /** gets the right edge (x + w).
    \note r() and b() are coordinates \b outside the area of the rectangle.
  */
  int r() const { return x_ + w_; }
  /** gets the bottom edge (y + h).
    \note r() and b() are coordinates \b outside the area of the rectangle.
  */
  int b() const { return y_ + h_; }

  void x(int X) { x_ = X; }             ///< sets the x coordinate (left edge)
  void y(int Y) { y_ = Y; }             ///< sets the y coordinate (top edge)
  void w(int W) { w_ = W; }             ///< sets the width
  void h(int H) { h_ = H; }             ///< sets the height

}; // class Fl_Rect

#endif // Fl_Rect_H
