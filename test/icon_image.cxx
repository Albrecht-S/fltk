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

int main(int argc, char **argv) {
  int id = 0;
  const char *name = "icon_image.ico";

  if (argc > 1)
    name = argv[1];
  if (argc > 2)
    id = atoi(argv[2]);

  Fl_ICO_Image *ico = new Fl_ICO_Image(name, NULL, -2);
  Fl_Double_Window *win = new Fl_Double_Window(300, 400, "Fl_ICO_Image Test");
  Fl_Box *b = new Fl_Box(0, 0, win->w(), win->h());

  printf("icon count: %d\n", ico->idcount());

  if (ico->idcount() < 1) {
    printf("No icon resources found\n");
    return 1;
  }

  if (id > ico->idcount() - 1) {
    printf("Icon #%d does not exist in file %s\n", id, name);
    return 2;
  }

  printf("icon resource #%d offset: %d\n", id, ico->icondirentry()[id].dwImageOffset);

  delete ico;
  ico = new Fl_ICO_Image(name, NULL, id);

  if (!ico->fail()) {
    float f1 = 128. / ico->w();
    float f2 = 128. / ico->h();
    if (f2 < f1)
      f1 = f2;
    Fl_Image *ico2 = ico->copy(int(f1 * ico->w()), int(f1 * ico->h()));
    b->image(ico2);
  }
  win->end();
  win->show();

  return Fl::run();
}
