//
// Simple demo of drawing an "X" in FLTK with Fl_Cairo2_Window (antialiased lines)
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
#include <FL/Fl_Cairo2_Window.H>
#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>

// Cairo rendering callback called by Fl_Cairo2_Window::draw()

static void my_cairo_draw_cb(Fl_Cairo2_Window *window, cairo_t *cr) {
  const double xmax = (window->w() - 1);
  const double ymax = (window->h() - 1);

  cairo_set_antialias(cr, CAIRO_ANTIALIAS_DEFAULT); // use default antialiasing

  // Draw orange "X"
  //     Draws an X to four corners of resizable window.
  //     See Fl_Cairo2_Window docs for more info.

  cairo_save(cr);
  cairo_set_line_width(cr, 2.5);            // line width for drawing
  cairo_set_source_rgb(cr, 1.0, 0.5, 0.0);  // orange
  cairo_move_to(cr, 0.0, 0.0);
  cairo_line_to(cr, xmax, ymax);            // draw diagonal "\"
  cairo_move_to(cr, 0.0, ymax);
  cairo_line_to(cr, xmax, 0.0);             // draw diagonal "/"
  cairo_stroke(cr);                         // stroke the lines
  cairo_restore(cr);

  // use FLTK drawing as well, drawing *over* Cairo drawings

  fl_color(0x88ddff00);                     // light blue/cyan
  fl_rectf(40, 40, 100, 25);
}

int main(int argc, char **argv) {
  Fl_Double_Window window(400, 400, "Cairo Draw 'X'");  // main window
  window.color(FL_YELLOW);                               // main window's default bg color

  Fl_Cairo2_Window cairo_win(50, 50, 300, 300);         // Fl_Cairo2_Window as subwindow
  cairo_win.set_draw_cb(my_cairo_draw_cb);              // draw callback for cairo drawing
  cairo_win.color(FL_GREEN);
  Fl_Box box1(30, 30, 240, 160, "White Fl_Box in green Fl_Cairo2_Window");
  box1.box(FL_FLAT_BOX);
  box1.color(FL_WHITE);
  box1.align(FL_ALIGN_INSIDE | FL_ALIGN_WRAP | FL_ALIGN_CLIP);

  cairo_win.tooltip("The orange X is drawn with Cairo");
  cairo_win.resizable(box1);
  cairo_win.end();

  window.size_range(350, 300);  // allow resize 350 x 300 and up
  window.resizable(&cairo_win); // allow window to be resized
  window.end();
  window.show(argc, argv);
  return Fl::run();
}
