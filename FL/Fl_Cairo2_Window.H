//
// Fl_Cairo2_Window header and implementation for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2023 by Bill Spitzak and others.
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

/* \file  FL/Fl_Cairo2_Window.H

  \brief  Fl_Cairo2_Window is an FLTK window incorporating a Cairo draw callback.

  This improved version of Fl_Cairo_Window is derived from Fl_Double_Window and
  uses the new class Fl_Cairo2 (since 1.4.0) to provide the Cairo context.

  \since 1.4.0
*/

#ifndef FL_CAIRO2_WINDOW_H
#define FL_CAIRO2_WINDOW_H

#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Cairo2.H>

/**
   \addtogroup group_cairo
   @{
*/

/**
  This defines an FLTK window with Cairo support.

  This class overloads the virtual draw() method for you,
  so that the only thing you have to do is to provide your Cairo code.
  All Cairo context handling is achieved transparently.

  The default coordinate system for Cairo drawing commands within Fl_Cairo2_Window
  is FLTK's coordinate system, where the `x,y,w,h` values are relative to the
  top/left corner of the Fl_Cairo2_Window, as one would expect with regular
  FLTK drawing commands, e.g.: `(0 ≤ x ≤ w-1),(0 ≤ y ≤ h-1)`.
  \b Example:
  \code
  static void my_cairo_draw_cb(Fl_Cairo2_Window *window, cairo_t *cr) {
      // Draw an "X"
      const double xmax = (window->w() - 1);
      const double ymax = (window->h() - 1);
      cairo_set_line_width(cr, 1.00);                               // line width for drawing
      cairo_set_source_rgb(cr, 1.0, 0.5, 0.0);                      // orange
      cairo_move_to(cr, 0.0, 0.0);  cairo_line_to(cr, xmax, ymax);  // draw diagonal "\"
      cairo_move_to(cr, 0.0, ymax); cairo_line_to(cr, xmax, 0.0);   // draw diagonal "/"
      cairo_stroke(cr);                                             // stroke the lines
  }
  \endcode

  The FLTK coordinate system differs from the default native Cairo coordinate system
  which uses normalized `(0.0 … 1.0)` values for x and y, e.g.:
  `(0 ≤ x ≤ 1.0),(0 ≤ y ≤ 1.0)`. So beware of this when copy/pasting Cairo example programs that
  assume normalized values. If need be, you can revert to the Cairo coordinate system by simply
  calling `cairo_scale()` with the widget's `w()` and `h()` values. \b Example:

  \code
  static void my_cairo_draw_cb(Fl_Cairo2_Window *window, cairo_t *cr) {
      cairo_scale(cr, window->w(), window->h());    // use Cairo's default coordinate system
      [..use 0.0 to 1.0 values from here on..]
  }
  \endcode

  \see FIXME: examples/cairo-draw-x.cxx
  \see FIXME: test/cairo_test.cxx

  \note Class Fl_Cairo2_Window does \b not require the FLTK library to be configured
    with CMake option OPTION_CAIRO or configure --enable-cairo.

  \note You can alternatively define your own Cairo FLTK window and use class Fl_Cairo2
    to acquire a Cairo context in your overridden draw() method and in any widget that
    uses Fl_Cairo2 to get the window's Cairo context.

  \see Fl_Cairo::make_current()
*/
class FL_EXPORT Fl_Cairo2_Window : public Fl_Double_Window {

public:
  Fl_Cairo2_Window(int W, int H, const char *L = 0)
    : Fl_Double_Window(W, H, L)
    , cairo_status_(this)
    , draw_cb_(0) {}

  Fl_Cairo2_Window(int X, int Y, int W, int H, const char *L = 0)
    : Fl_Double_Window(X, Y, W, H, L)
    , cairo_status_(this)
    , draw_cb_(0) {}

protected:
  /** Overloaded to provide Cairo callback support */
  void draw() FL_OVERRIDE {
    Fl_Double_Window::draw();

    if (draw_cb_) {
      // call the Cairo draw callback
      draw_cb_(this, cairo_status_.make_current());

      // flush Cairo drawings: necessary at least for Windows
      cairo_status_.flush();
    }
  } // draw()

public:
  /** The Cairo draw callback prototype you need to implement. */
  typedef void (*cairo2_draw_cb)(Fl_Cairo2_Window *self, cairo_t *def);

  /**
    You must provide a draw callback that implements your Cairo rendering.
    This method sets your Cairo callback to \p cb.
  */
  void set_draw_cb(cairo2_draw_cb cb) { draw_cb_ = cb; }

  /**
    Flush Cairo rendering immediately when called.

    You may want to use this method if you are drawing with both FLTK and Cairo
    sequentially. This should be rarely needed.

    \note Fl_Cairo2_Window flushes the Cairo drawings for you automatically after
      the draw callback has finished.

    \see Fl_Cairo2::flush()
  */
  void cairo_flush() {
    cairo_status_.flush();
  }

private:
  Fl_Cairo2 cairo_status_;
  cairo2_draw_cb draw_cb_;
};


/** @} */

#endif // FL_CAIRO2_WINDOW_H
