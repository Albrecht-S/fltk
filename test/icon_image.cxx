//
// Fl_ICO_Image test program for the Fast Light Tool Kit (FLTK).
//
// Copyright 2020 by Bill Spitzak and others.
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

// This test program loads an icon image (.ico) and allows to switch
// the loaded icon by its id in the icon file

#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_ICO_Image.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Simple_Counter.H>
#include <FL/filename.H>

// Globals to simplify the test code

const char *name = 0;                 // default icon (file) name
Fl_Double_Window *win = 0;            // main window
Fl_Box *box = 0;                      // box used to display the icon
Fl_Box *info = 0;                     // icon information
const int scale = 0;                  // optional icon scaling to N x N pixels (max. 256)

// Load or reload icon by name and icon id

void load_icon(const char *name, int id = 0) {

  if (box->image())
    delete box->image();
  box->image(0);

  Fl_ICO_Image *ico = new Fl_ICO_Image(name, NULL, id);
  if (ico->fail()) {
    info->label("Can't load icon file");
  }

  fl_ICONDIRENTRY *icondir = &ico->icondirentry()[id];
  static char buf[40];
  sprintf(buf, "%d x %d px, offset %d, size %d",
          icondir->bWidth, icondir->bHeight, icondir->dwImageOffset, icondir->dwBytesInRes);
  info->label(buf);

  Fl_Image *img = 0;
  if (scale > 0) {
    float f1 = float(scale) / ico->w();
    float f2 = float(scale) / ico->h();
    if (f2 < f1) f1 = f2;
    img = ico->copy(int(f1 * ico->w()), int(f1 * ico->h()));
  } else {
    img = ico->copy(ico->w(), ico->h());
  }
  box->image(img);

  delete ico;
  box->redraw();
}

// Fl_Counter callback to change icon (id) and load the new icon.
// The Fl_Counter widget is set up with appropriate min and max values

void icon_cb(Fl_Widget *w, void *b) {
  Fl_Simple_Counter *ct = (Fl_Simple_Counter *)w;
  load_icon(name, int(ct->value()));
}

// Main program

int main(int argc, char **argv) {
  int i;
  if (!Fl::args(argc, argv, i)) Fl::fatal(Fl::help);
  name = (i < argc) ? argv[i] : "icon_image.ico";

  win = new Fl_Double_Window(300, 400, "Fl_ICO_Image");
  box = new Fl_Box(22, 22, 256, 256);
  box->box(FL_FLAT_BOX);
  box->color(FL_WHITE);

  // open icon file, get icon count ...

  Fl_ICO_Image *ico = new Fl_ICO_Image(name, NULL, -2);
  int icon_count = ico->idcount();
  fprintf(stderr, "icon count: %d\n", icon_count);
  delete ico;

  if (icon_count < 1) {
    fprintf(stderr, "No icon resources found\n");
    return 1;
  }

  // Simple counter to select icon index for viewing

  Fl_Simple_Counter *ct = new Fl_Simple_Counter(100, 300, 100, 25, "Icon # ");
  ct->align(FL_ALIGN_LEFT);
  ct->callback(icon_cb, (void*)box);
  ct->minimum(0);
  ct->step(1);
  ct->maximum(icon_count - 1);
  ct->value(0);

  Fl_Box *num = new Fl_Box(220, 300, 100, 25);
  num->box(FL_FLAT_BOX);
  num->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);

  static char icount[20];
  sprintf(icount, "(0 - %d)", icon_count - 1);
  num->label(icount);

  // icon info display

  info = new Fl_Box(10, 350, 280, 25);
  info->box(FL_FLAT_BOX);
  info->align(FL_ALIGN_CENTER | FL_ALIGN_INSIDE);

  win->label(fl_filename_name(name));
  load_icon(name);

  win->end();
  win->resizable(box);
  win->show();

  return Fl::run();
}
