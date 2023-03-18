//
// New Cairo drawing test program for the Fast Light Tool Kit (FLTK).
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

#include <FL/Fl.H>
#include <FL/Fl_Cairo2.H>

#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>
#include <FL/math.h>

#define DEF_WIDTH 0.03

// This demo program can be used w/o configuring FLTK for Cairo usage.

// For simplicity we use a global Cairo context because we're using only one window.

Fl_Cairo2 *cs = 0;

// draw centered text with Cairo

static void centered_text(cairo_t *cr, double x0, double y0, double w0, double h0, const char *my_text) {
  cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_OBLIQUE, CAIRO_FONT_WEIGHT_BOLD);
  cairo_set_source_rgba(cr, 0.9, 0.9, 0.4, 0.6);
  cairo_text_extents_t extents;
  cairo_text_extents(cr, my_text, &extents);
  double x = (extents.width / 2 + extents.x_bearing);
  double y = (extents.height / 2 + extents.y_bearing);
  cairo_move_to(cr, x0 + w0 / 2 - x, y0 + h0 / 2 - y);
  cairo_text_path(cr, my_text);
  cairo_fill_preserve(cr);
  cairo_set_source_rgba(cr, 0, 0, 0, 1);
  cairo_set_line_width(cr, 0.004);
  cairo_stroke(cr);
  cairo_set_line_width(cr, DEF_WIDTH);
}

// draw a button object with rounded corners and a label

static void round_button(cairo_t *cr, double x0, double y0,
                         double rect_width, double rect_height, double radius,
                         double r, double g, double b) {
  double x1, y1;
  x1 = x0 + rect_width;
  y1 = y0 + rect_height;
  if (!rect_width || !rect_height)
    return;
  if (rect_width / 2 < radius) {
    if (rect_height / 2 < radius) {
      cairo_move_to(cr, x0, (y0 + y1) / 2);
      cairo_curve_to(cr, x0, y0, x0, y0, (x0 + x1) / 2, y0);
      cairo_curve_to(cr, x1, y0, x1, y0, x1, (y0 + y1) / 2);
      cairo_curve_to(cr, x1, y1, x1, y1, (x1 + x0) / 2, y1);
      cairo_curve_to(cr, x0, y1, x0, y1, x0, (y0 + y1) / 2);
    } else {
      cairo_move_to(cr, x0, y0 + radius);
      cairo_curve_to(cr, x0, y0, x0, y0, (x0 + x1) / 2, y0);
      cairo_curve_to(cr, x1, y0, x1, y0, x1, y0 + radius);
      cairo_line_to(cr, x1, y1 - radius);
      cairo_curve_to(cr, x1, y1, x1, y1, (x1 + x0) / 2, y1);
      cairo_curve_to(cr, x0, y1, x0, y1, x0, y1 - radius);
    }
  } else {
    if (rect_height / 2 < radius) {
      cairo_move_to(cr, x0, (y0 + y1) / 2);
      cairo_curve_to(cr, x0, y0, x0, y0, x0 + radius, y0);
      cairo_line_to(cr, x1 - radius, y0);
      cairo_curve_to(cr, x1, y0, x1, y0, x1, (y0 + y1) / 2);
      cairo_curve_to(cr, x1, y1, x1, y1, x1 - radius, y1);
      cairo_line_to(cr, x0 + radius, y1);
      cairo_curve_to(cr, x0, y1, x0, y1, x0, (y0 + y1) / 2);
    } else {
      cairo_move_to(cr, x0, y0 + radius);
      cairo_curve_to(cr, x0, y0, x0, y0, x0 + radius, y0);
      cairo_line_to(cr, x1 - radius, y0);
      cairo_curve_to(cr, x1, y0, x1, y0, x1, y0 + radius);
      cairo_line_to(cr, x1, y1 - radius);
      cairo_curve_to(cr, x1, y1, x1, y1, x1 - radius, y1);
      cairo_line_to(cr, x0 + radius, y1);
      cairo_curve_to(cr, x0, y1, x0, y1, x0, y1 - radius);
    }
  }
  cairo_close_path(cr);

  cairo_pattern_t *pat =
    cairo_pattern_create_radial(0.25, 0.24, 0.11, 0.24, 0.14, 0.35);
  cairo_pattern_set_extend(pat, CAIRO_EXTEND_REFLECT);

  cairo_pattern_add_color_stop_rgba(pat, 1.0, r, g, b, 1);
  cairo_pattern_add_color_stop_rgba(pat, 0.0, 1, 1, 1, 1);
  cairo_set_source(cr, pat);
  cairo_fill_preserve(cr);
  cairo_pattern_destroy(pat);

  cairo_set_source_rgba(cr, 0, 0, 0.5, 0.3);
  cairo_stroke(cr);

  cairo_set_font_size(cr, 0.075);
  centered_text(cr, x0, y0, rect_width, rect_height, "FLTK loves Cairo2 !");
}

// draw the entire image (3 buttons), scaled to the given width and height

void draw_image(cairo_t *cr, int w, int h) {

  cairo_save(cr);
  cairo_set_line_width(cr, DEF_WIDTH);
  cairo_scale(cr, w, h);

  round_button(cr, 0.1, 0.1, 0.8, 0.2, 0.4, 1, 0, 0);
  round_button(cr, 0.1, 0.4, 0.8, 0.2, 0.4, 0, 1, 0);
  round_button(cr, 0.1, 0.7, 0.8, 0.2, 0.4, 0, 0, 1);

  cairo_restore(cr);

} // draw_image()

class cairo_window : public Fl_Double_Window {

public:

  cairo_window(int w, int h, const char *title) : Fl_Double_Window(w, h, title) {
    Fl_Box *box = new Fl_Box(FL_NO_BOX, 0, 0, w, 25,
                             "Cairo and FLTK API in Fl_Double_Window");
    box->labelfont(FL_TIMES_BOLD);
    box->labelsize(12);
    box->labelcolor(FL_BLUE);
  }
  void draw() FL_OVERRIDE {
    Fl_Window::draw(); // perform drawings with the FLTK API

    if (!cs)
      cs = new Fl_Cairo2(this);       // create Cairo2 object
    cairo_t *cc = cs->make_current(); // get the Cairo context

    draw_image(cc, w(), h());         // draw in this window using Cairo

    cs->flush();                      // flush Cairo drawings
  }

}; // class cairo_window

int main(int argc, char **argv) {

  cairo_window window(350, 350, "FLTK loves Cairo2");

  window.resizable(&window);
  window.color(FL_WHITE);
  window.show(argc, argv);

  return Fl::run();
}
