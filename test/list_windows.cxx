// This file must be #include'd ...

#include <FL/Fl.H>
#include <FL/platform.H>

void list_windows(void *v) {
  static int c = 0;
  auto w = Fl::first_window();
  int n = 0;
  c = (c + 1) % 10;
  fprintf(stderr, "-- %d --\nn: window         |shown| vis.| par.|bord.| max.|full.| label                  |\n", c);
  while (w) {
    fprintf(stderr, "%d: %p |%3d  |%3d  |%3d  |%3d  |%3d  |%3d  | %-22.22s |\n",
            n, w, w->shown(), w->visible(), w->parent() ? 1 : 0,
	    w->border(),
	    w->maximize_active() ? 1 : 0,
	    w->fullscreen_active() ? 1 : 0,
            w->label());
    w = Fl::next_window(w);
    n++;
  }
  Fl::repeat_timeout(3.0, list_windows);
}
