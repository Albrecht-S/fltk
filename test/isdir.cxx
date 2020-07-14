//
// Test fl_filename_isdir() for the Fast Light Tool Kit (FLTK).
//
// See code for usage...

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/filename.H>
#include <stdio.h>

const char *files[] = {
#include "files.dat"
  "no-directory",
  NULL
};

int main(int argc, char **argv) {
  const char *f = "../äöü";
  const char *res;
  if (argc > 1)
    f = argv[1];
  int dir = fl_filename_isdir(f);
  f = files[0];
  for (int n = 0; n < 10000 && f; n++) {
    dir = fl_filename_isdir(f);
    res = dir ? "true" : "false";
    printf("[%4d] fl_filename_isdir(\"%s\") = %d (%s)\n", n, f, dir, res);
    f = files[n];
  }

#if 0
  argc = 1;
  Fl_Window *window = new Fl_Window(340,180, res);
  Fl_Box *box = new Fl_Box(20,40,300,100,f);
  box->box(FL_UP_BOX);
  box->labelfont(FL_BOLD+FL_ITALIC);
  box->labelsize(36);
  box->labeltype(FL_SHADOW_LABEL);
  window->end();
  window->show(argc, argv);
#endif
  return 0; // Fl::run();
}
