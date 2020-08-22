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

#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_ICO_Image.H>
#include <FL/Fl_Double_Window.H>

void load_icon(Fl_Box *box, const char *name, int id, int scale = 0) {
  if (box->image())
    delete box->image();
  box->image(0);
  Fl_ICO_Image *ico = new Fl_ICO_Image(name, NULL, id);
  if (!ico->fail()) {
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
  }
  delete ico;
}

int main(int argc, char **argv) {
  int id = 0;
  const char *name = "icon_image.ico";

  if (argc > 1)
    name = argv[1];
  if (argc > 2)
    id = atoi(argv[2]);

  Fl_Double_Window *win = new Fl_Double_Window(300, 400, "Fl_ICO_Image Test");
  Fl_Box *box = new Fl_Box(0, 0, win->w(), win->h());

  Fl_ICO_Image *ico = new Fl_ICO_Image(name, NULL, -2);
  int icon_count = ico->idcount();
  printf("icon count: %d\n", icon_count);

  if (icon_count < 1) {
    printf("No icon resources found\n");
    return 1;
  }

  if (id > icon_count - 1) {
    printf("Icon #%d does not exist in file %s\n", id, name);
    return 2;
  }

  fl_ICONDIRENTRY *icondir = &ico->icondirentry()[id];
  printf("icon resource #%d offset: %d, width: %d, height: %d\n",
         id, icondir->dwImageOffset, icondir->bWidth, icondir->bHeight);

  delete ico;
  load_icon(box, name, id);

  win->end();
  win->show();

  return Fl::run();
}
