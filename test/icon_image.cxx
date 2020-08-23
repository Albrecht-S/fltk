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
// the loaded icon by its id to view all icons in an icon file.
// The window icon is changed when a new icon is loaded (if possible).

#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_ICO_Image.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Simple_Counter.H>
#include <FL/filename.H>
#include <FL/Fl_File_Chooser.H>

#include <stdio.h>

// Globals to simplify the test code

const char *icon_name = 0;     // icon (file) name
Fl_Double_Window *win = 0;     // main window
Fl_Box *box = 0;               // box used to display the icon
Fl_Box *info = 0;              // icon information box
Fl_Box *num = 0;               // info: "(0 - n)"
Fl_Simple_Counter *ct = 0;     // icon counter (0..icon_count-1)
int icon_count = 0;            // number of icon images in icon file

// Load or reload icon by name and icon id

void load_icon(const char *name, int id = 0) {

  if (box->image())
    delete box->image();
  box->image(0);

  Fl_ICO_Image *ico = new Fl_ICO_Image(name, NULL, id);
  if (ico->fail()) {
    info->label("Can't load icon file");
    box->redraw();
    return;
  }

  fl_ICONDIRENTRY *icondir = &ico->icondirentry()[id];
  static char buf[40];
  sprintf(buf, "%d x %d px, size %d, offset %d",
          icondir->bWidth, icondir->bHeight, icondir->dwBytesInRes, icondir->dwImageOffset);
  info->label(buf);

  box->image(ico);
  box->redraw();
  win->icon(ico); // does not always work with all WM's / on all platforms
}

// Fl_Counter callback to change icon (id) and load the new icon.
// The Fl_Counter widget is set up with appropriate min and max values

void icon_cb(Fl_Widget *w, void *b) {
  Fl_Simple_Counter *ct = (Fl_Simple_Counter *)w;
  load_icon(icon_name, int(ct->value()));
}

// Load a new icon file, initialize icon data etc.,
// then load the first icon.
// Reset info but do nothing else if fname == NULL.

void load_icon_file(const char *fname) {

  free((void *)icon_name);
  icon_name = 0;
  icon_count = 0;

  if (box->image())
    delete box->image();
  box->image(0);
  box->redraw();

  info->label("no icon file loaded");
  info->redraw();

  if (!fname) {
    win->label("Fl_ICO_Image");
    return;
  }

  icon_name = strdup(fname);
  win->label(fl_filename_name(icon_name));

  // open icon file, get icon count ...

  Fl_ICO_Image *ico = new Fl_ICO_Image(icon_name, NULL, -2);
  icon_count = ico->idcount();
  // fprintf(stderr, "icon count: %d\n", icon_count);
  delete ico;

  int icon_max = icon_count > 0 ? icon_count - 1 : 0;
  ct->maximum(icon_max);
  ct->value(0);

  static char icount[20];
  if (icon_count > 0)
    sprintf(icount, "(0 - %d)", icon_max);
  else
    strcpy(icount, "(no icon)");
  num->label(icount);

  if (icon_count < 1) {
    // fprintf(stderr, "No icon resources found\n");
    return;
  }

  ct->take_focus();
  load_icon(icon_name);
}

// Button callback to load a new .ico file

void load_cb(Fl_Widget *, void *) {
  const char *fname = fl_file_chooser("Icon file?", "*.ico", icon_name);
  load_icon_file(fname);
}

// Main program

int main(int argc, char **argv) {
  int i;
  if (!Fl::args(argc, argv, i)) Fl::fatal(Fl::help);
  const char *iname = (i < argc) ? argv[i] : "icon_image.ico";

  win = new Fl_Double_Window(300, 410, "Fl_ICO_Image");
  box = new Fl_Box(22, 22, 256, 256);
  box->box(FL_FLAT_BOX);
  box->color(FL_WHITE);

  // Simple counter to select icon index for viewing

  ct = new Fl_Simple_Counter(100, 300, 100, 25, "Icon # ");
  ct->align(FL_ALIGN_LEFT);
  ct->callback(icon_cb, (void*)box);
  ct->minimum(0);
  ct->step(1);
  ct->maximum(0);
  ct->value(0);

  num = new Fl_Box(220, 300, 100, 25);
  num->box(FL_FLAT_BOX);
  num->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);

  // icon info

  info = new Fl_Box(10, 335, 280, 25);
  info->box(FL_FLAT_BOX);
  info->align(FL_ALIGN_CENTER | FL_ALIGN_INSIDE);

  // load button (new file)

  Fl_Button *load = new Fl_Button(50, 370, 200, 25, "Load icon (.ico) file");
  load->callback(load_cb);

  win->end();
  win->resizable(box);
  win->show();

  // open icon file, get icon count ...

  load_icon_file(iname);

  return Fl::run();
}
