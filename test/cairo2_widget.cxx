//
// Cairo widget drawing test program for the Fast Light Tool Kit (FLTK).
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
#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>

// This demo program can be used w/o configuring FLTK for Cairo usage.

// draw a filled rectangle with black border

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
  cairo_set_line_width(cr, 2.0);
  cairo_stroke_preserve(cr);

  // fill rectangle with the given color

  cairo_set_source_rgba(cr, r, g, b, a);
  cairo_fill(cr);

  cairo_restore(cr);
}


class cairo_widget : public Fl_Box {

  float alpha_;

public:
  cairo_widget(int x, int y, int w, int h, const char *title)
    : Fl_Box(x, y, w, h, title) {
    labelfont(FL_COURIER_BOLD);
    labelsize(16);
    labelcolor(FL_BLUE);
    box(FL_NO_BOX);       // box is drawn using Cairo
    alpha_ = 1.0;
  }

protected:
  void draw() FL_OVERRIDE {

    static int nnn = 0;
    printf("cairo_widget::draw[%4d]\n", ++nnn); fflush(stdout);

    Fl_Cairo2 cs(window());          // Cairo status: use local variable
    cairo_t *cc = cs.make_current(); // get the Cairo context
    uchar r, g, b;
    Fl::get_color(color(), r, g, b);

    // draw the button
    draw_rect(cc, x(), y(), w(), h(), r / 255., g / 255., b / 255., alpha_);

    // flush Cairo drawings before drawing the label
    cs.flush();

    // draw the label with FLTK's standard method (using fl_contrast())

    Fl_Color saved_color = labelcolor();
    labelcolor(fl_contrast(labelcolor(), color()));
    draw_label();
    labelcolor(saved_color);

    // Fl_Cairo2 cs goes out of scope here, destroying the Cairo context
  }

public:
  void alpha(float a) {
    alpha_ = a;
    if (alpha_ < 0.0)
      alpha_ = 0.0;
    if (alpha_ > 1.0)
      alpha_ = 1.0;
  }

  float alpha() { return alpha_; }

}; // class cairo_widget

int main(int argc, char **argv) {

  Fl_Double_Window window(300, 300, "FLTK Cairo Widgets");

  cairo_widget cb1(10, 10, 120, 120, "Box 1");
  cb1.color(FL_RED);
  cb1.alpha(0.7);

  cairo_widget cb2(90, 90, 120, 120, "Box 2");
  cb2.color(FL_GREEN);
  cb2.alpha(0.6);

  cairo_widget cb3(170, 170, 120, 120, "Box 3");
  cb3.color(FL_BLUE);
  cb3.alpha(0.5);

  window.resizable(&window);
  window.color(FL_WHITE);
  window.show(argc, argv);

  return Fl::run();
}
