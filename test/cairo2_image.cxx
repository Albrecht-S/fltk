//
// Cairo image drawing test program for the Fast Light Tool Kit (FLTK).
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

#include <FL/Fl.H>
#include <FL/platform.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Copy_Surface.H>
#include <FL/Fl_Image_Surface.H>
#include <FL/Fl_Image.H>
#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>

#include <cairo.h>

// include platform specific Cairo headers (if necessary)

#if defined(_WIN32)               // Windows
#include <cairo-win32.h>
#elif defined(FLTK_USE_X11)       // X11 with or w/o Wayland
#include <cairo-xlib.h>
#elif defined(__APPLE__)          // macOS
#include <cairo-quartz.h>
#elif defined(FLTK_USE_WAYLAND)   // Wayland: nothing to include
#include "../src/drivers/Wayland/Fl_Wayland_Graphics_Driver.H"
#include "../src/drivers/Wayland/Fl_Wayland_Window_Driver.H"
static void *fl_gc = 0;           // dummy fl_gc for "Wayland only" configuration
#else
#error Cairo is not supported on this platform.
#endif


// This demo program can be used w/o configuring FLTK for Cairo.
// Its purpose is to show how to draw on an Fl_Image_Surface using Cairo.

// Create a Cairo context that can be used on an FLTK image surface.
// In this demo we assume that the surface is the "current surface" after
// 'Fl_Surface_Device::push_current(surf)'.

cairo_t *cairo_context(Fl_Widget_Surface *surf, int W, int H) {

  cairo_surface_t *cairo_surface = NULL;
  cairo_t *cairo_context = NULL;

  // Windows

#if defined(_WIN32)
  cairo_surface = cairo_win32_surface_create(fl_win32_gc());
  cairo_context = cairo_create(cairo_surface);
  cairo_surface_destroy(cairo_surface);
  return cairo_context;
#endif

  // Wayland

#ifdef FLTK_USE_WAYLAND
  if (fl_wl_display()) { // to avoid when the hybrid library is in x11 mode
#if (1) // NOT implemented
    cairo_context = NULL;
#else
    struct wld_window *xid = fl_wl_xid(fl_window);
    if (!xid->buffer)
      return NULL; // this may happen with GL windows
    cairo_context = xid->buffer->draw_buffer.cairo_;
#endif // NOT implemented
    return cairo_context;
  }
#endif // FLTK_USE_WAYLAND

  // X11

#if defined(FLTK_USE_X11)
  cairo_surface = cairo_xlib_surface_create(fl_display, fl_window, fl_visual->visual, W, H);
  cairo_context = cairo_create(cairo_surface);
  cairo_surface_destroy(cairo_surface);
  return cairo_context;
#endif

  // macOS

#if defined(__APPLE__) && !defined(FLTK_USE_X11)
#include <ApplicationServices/ApplicationServices.h>
#include <cairo-quartz.h>
  CGContextRef gc = fl_mac_gc();
  cairo_surface = cairo_quartz_surface_create_for_cg_context(gc, W, H);
  cairo_context = cairo_create(cairo_surface);
  cairo_surface_destroy(cairo_surface);
  return cairo_context;
#endif

  // remaining platforms (shouldn't be any)

  return NULL;

} // cairo_context(Fl_Image_Surface *surf)


// draw a filled rectangle with black border using Cairo

static void draw_rect(cairo_t *cr,
                      double x0, double y0, double w0, double h0,
                      double r, double g, double b, double a) {
  cairo_save(cr);

  // create path

  cairo_move_to(cr, x0 + 1, y0 + 1);
  cairo_line_to(cr, x0 + w0 - 1, y0 + 1);
  cairo_line_to(cr, x0 + w0 - 1, y0 + h0 - 1);
  cairo_line_to(cr, x0 + 1, y0 + h0 - 1);
  cairo_close_path(cr);

  // draw black border

  cairo_set_source_rgba(cr, 0, 0, 0, 1);
  cairo_set_line_width(cr, 1.5);
  cairo_stroke_preserve(cr);

  // fill rectangle with the given color

  cairo_set_source_rgba(cr, r, g, b, a);
  cairo_fill(cr);

  // flush Cairo drawings

  cairo_surface_t *s = cairo_get_target(cr);
  cairo_surface_flush(s);

  cairo_restore(cr);
}

// Copy a rectangle to the clipboard

void copy_rect(int W, int H, Fl_Color col) {

  printf("copy_rect( %3d x %3d, color: %2d )\n", W, H, (int)col);

  // use Fl_Copy_Surface to copy an image to the clipboard

  Fl_Copy_Surface *copy_surf = new Fl_Copy_Surface(W, H);
  Fl_Surface_Device::push_current(copy_surf);

  fl_rectf(0, 0, W, H, FL_YELLOW);        // yellow background
  fl_rect(0, 0, W, H);                    // black border
  fl_rectf(10, 10, W - 20, H - 20, col);  // a rectangle in the given color

  // draw with Cairo ...
  cairo_t *cc = cairo_context(copy_surf, W, H);
  if (cc) {
    draw_rect(cc, 21, 21, 39, 39, 1, 0, 1, 1); // pink rectangle with Cairo
  }

  delete copy_surf;
  Fl_Surface_Device::pop_current();
}


// Timer callback: repeatedly copy a rectangle in a different color.
// This test uses Fl_Copy_Surface with FLTK and Cairo drawing.

void copy_cb(void *) {

  static int color = 0;
  color++;
  color &= 15;
  static int width = 90;
  width += 10;
  if (width > 200) width = 100;
  copy_rect(width, width, (Fl_Color)color);
  Fl::repeat_timeout(1.0, copy_cb);
} // copy_cb()


// Create an Fl_RGB_Image with content drawn by FLTK and Cairo

Fl_RGB_Image *create_image(int W, int H) {

  Fl_Image_Surface *surf = new Fl_Image_Surface(W, H);
  Fl_Surface_Device::push_current(surf);

  // fill the background
  fl_color(FL_WHITE);
  fl_rectf(0, 0, W, H);

  // draw a red frame
  fl_color(FL_RED);
  fl_line_style(FL_SOLID, 2);
  fl_rect(1, 1, W - 2, H - 2);

  // draw a smaller rectangle in green color

  fl_color(FL_GREEN);
  fl_rectf(10, 10, W - 20, H - 20);

  // draw a blue rectangle using Cairo (!)

  cairo_t *cr = cairo_context(surf, W, H);
  if (cr) {
    draw_rect(cr, 21, 21, 39, 39, 0.5, 0.5, 1.0, 0.8);
  }

  Fl_RGB_Image *img = surf->image();
  surf->pop_current();
  delete surf;
  return img;
}

// Main program: assign an image (potentially created using Cairo)
// to an Fl_Box widget

int main(int argc, char **argv) {

  const int W = 100;
  const int H = 100;

  fl_open_display();

  Fl_Double_Window window(300, 300, "FLTK Cairo Image");

  // create an image with Cairo and assign it to an Fl_Box widget

  Fl_RGB_Image *img = create_image(W, H);   // create image for ...
  Fl_Box imgbox(100, 100, W, H);            // image box
  imgbox.image(img);                        // assign image to box

  copy_rect(240, 240, FL_RED);
  Fl::add_timeout(5.0, copy_cb);

  window.resizable(&window);
  window.color(0xffffdd00); // light yellow
  window.show(argc, argv);

  return Fl::run();
}
