//
// "New" Cairo support header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2024 by Bill Spitzak and others.
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

/** \file Fl_Cairo2.H

  Cairo is currently supported for the following platforms:
    Windows, macOS, Unix/Linux (X11 + Wayland).

  This new version of Cairo support enables users to draw with Cairo into
  all their windows and widgets with a stock FLTK configuration.

  Using configure --enable-cairo, --enable-cairoext and/or the CMake options
  OPTION_CAIRO and OPTION_CAIROEXT are not needed and are deprecated since
  FLTK 1.4.0.

  \since 1.4.0
*/

#ifndef FL_CAIRO2_H
#define FL_CAIRO2_H

#include <FL/Fl.H>
#include <FL/platform.H>
#include <FL/Fl_Window.H>

// include platform specific Cairo headers

#include <cairo.h>

#if defined(_WIN32)               // Windows
#include <cairo-win32.h>
#elif defined(FLTK_USE_X11)       // X11 w/o Wayland
#include <cairo-xlib.h>
#elif defined(__APPLE__)          // macOS
#include <cairo-quartz.h>
#elif defined(FLTK_USE_WAYLAND)   // Wayland: nothing to include, see below
#  include "../src/drivers/Wayland/Fl_Wayland_Graphics_Driver.H"
#  include "../src/drivers/Wayland/Fl_Wayland_Window_Driver.H"
static void *fl_gc = 0;           // dummy fl_gc for "Wayland only" configuration
#else
#error Cairo is not supported on this platform.
#endif

/**
  Class Fl_Cairo2 stores everything necessary to keep the Cairo context
  of a window in sync with the window size, scaling, etc.

  You need to allocate one Fl_Cairo2 object per window.


  This class and its implementation is entirely contained in this header file.
  \code
  #include <FL/Fl_Cairo2.H>
  \endcode

  More to come ...

  \since 1.4.0
*/
class FL_EXPORT Fl_Cairo2 {

protected:
  cairo_t *cairo_context;
  cairo_surface_t *cairo_surface;

private:
  Fl_Window *window_;
  void *gc_;
  float scale_;
  int width_;
  int height_;

public:
  Fl_Cairo2(Fl_Window *win)
    : cairo_context(0)
    , cairo_surface(0)
    , window_(win)
    , gc_(0)
    , scale_(1.0)
    , width_(0)
    , height_(0) {
  }

  virtual ~Fl_Cairo2() {
    static int nn = 0;
    printf("~Fl_Cairo2[%4d] ... ", ++nn); fflush(stdout);
#if defined(FLTK_USE_WAYLAND)
    if (fl_wl_display()) { // true means using wayland backend
      printf("using Wayland: do nothing.\n"); fflush(stdout);
      return;
    }
#endif
    if (cairo_context) {
      printf("destroy Cairo context and surface.\n"); fflush(stdout);
      cairo_destroy(cairo_context);
      cairo_surface_destroy(cairo_surface);
    } else {
      printf("no Cairo context, do nothing.\n"); fflush(stdout);
    }
  }

  /**
    Return the Cairo context of the associated window.

    This method creates a new Cairo context only if necessary and returns
    the Cairo context. It returns the previous context if nothing changed.

    The created Cairo context is scaled and sized appropriately for the
    given window and the system scaling factor. If you scale or transform
    the Cairo context for your drawings you need to save and restore the
    given Cairo context for future use.

    Typical Cairo drawing example:
    \code
      Fl_Cairo2 *cs = new Fl_Cairo2(win); // do this once and save it
      // every time you draw with Cairo
      cairo_t *cc = cs->make_current();   // get the Cairo context
      cc->cairo_save();                   // save the Cairo status
      // do your Cairo drawings here ...
      cc->cairo_restore();                // restore Cairo status
      cs->flush();                        // flush Cairo drawings
    \endcode

    \return  Cairo context for window
    \see Fl_Cairo2::flush()
  */
  cairo_t *make_current() {

    if (!window_)
      return NULL;

    // Wayland is a special case:
    // - always use the existing Wayland context
    // - never destroy it
    // - don't scale it

#if defined(FLTK_USE_WAYLAND)
    if (fl_wl_display()) { // true means using wayland backend
      struct wld_window *xid = fl_wl_xid(window_);
      if (!xid->buffer)
        return NULL; // this may happen with GL windows
      cairo_context = xid->buffer->draw_buffer.cairo_;
      return cairo_context;
    }
#endif

    // all other platforms: return the existing context if it is
    // the same gc/window combination, size, and scaling factor

    float scale = Fl::screen_scale(window_->screen_num());

    if (cairo_context) {
      if (window_->w() == width_ &&
          window_->h() == height_ &&
          fl_gc == gc_ &&
          int(scale * 1000) == scale_ * 1000) // avoid float comparison
        return cairo_context;
      else {
        // destroy the old context
        cairo_destroy(cairo_context);
        cairo_surface_destroy(cairo_surface);
      }
    }

    // set up the new context and store some details

    gc_     = fl_gc;
    width_  = window_->w();
    height_ = window_->h();
    scale_  = scale;

    // create the Cairo surface

#if defined(FLTK_USE_X11)
    cairo_surface = cairo_xlib_surface_create(fl_display, fl_window, fl_visual->visual,
                                              width_ * scale_, height_ * scale_);
#elif defined(_WIN32)
    cairo_surface = cairo_win32_surface_create((HDC)gc_);
#elif defined(__APPLE__)
    cairo_surface = cairo_quartz_surface_create_for_cg_context((CGContextRef)gc_, width_, height_);
#endif

    cairo_context = cairo_create(cairo_surface);

#ifndef __APPLE__
    cairo_scale(cairo_context, scale_, scale_);
#endif

    return cairo_context;

  } // make_current()

  /***
    Flush Cairo drawings to the device (screen, window).

    This is necessary after using Cairo to draw on the Windos platform
    but is recommended (and harmless) to do on all other platforms.

    Nothing is done if the Cairo context has not yet been initialized.
  */
  void flush() {
    if (!cairo_context)
      return;
    cairo_surface_t *s = cairo_get_target(cairo_context);
    cairo_surface_flush(s);
  }


}; // class Fl_Cairo2

#endif // FL_CAIRO2_H
