//
// Subwindow test program for the Fast Light Tool Kit (FLTK).
//
// This program "converts" a window from top level to subwindow
// and vice versa.
// It also shows a borderless window.

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>

Fl_Window*    win1;       // main window
Fl_Button*    embut;      // "embed button"
Fl_Window*    win2;       // other window
Fl_Box*       box2;       // box in win2
Fl_Window*    win3;       // 3rd, borderless window

void close_cb(Fl_Widget*, void*) {
  Fl::hide_all_windows();
}

void embed_cb(Fl_Widget* w, void* v) {
  win2->hide();
  if (win2->parent()) {       // win2 is subwindow, make it top level
    win1->remove(win2);
    win2->resize(200, 10, 400, 400);
    win2->color(FL_YELLOW);
    box2->label("Top Level Window");
    embut->label("Embed win2");

    // The following line should not be necessary, but it is required
    // on macOS and Wayland as of git commit ee3ba84aef99 (2026-06-03).
    // Leave it commented out to see the platform specific difference.

    // win2->border(1);       // needed by Wayland and macOS

  } else {                    // win2 is top level, embed it as subwindow
    win1->add(win2);
    win2->resize(10, 300, win1->w() - 20, 200);
    box2->resize(20, win2->h()/2 - 40, win2->w() - 40, 80);
    win2->init_sizes();
    win2->box(FL_UP_BOX);
    win2->color(FL_GREEN);
    box2->label("Subwindow");
    embut->label("Make win2 top level");
  }
  win2->show();
}

int main(int argc, char **argv) {

  win1 = new Fl_Window(600, 600, "win1 - main window");
  embut = new Fl_Button(20, 100, 560, 60, "Embed win2");
  embut->labelsize(20);
  win1->end();

  win2 = new Fl_Window(200, 100, 400, 400, "win2 - top level");
  win2->box(FL_UP_BOX);
  win2->color(FL_GREEN);
  box2 = new Fl_Box(50, 50, 300, 50, "Top Level Window");
  box2->box(FL_FLAT_BOX);
  box2->labelfont(FL_BOLD + FL_ITALIC);
  box2->labelsize(20);
  win2->resizable(box2);
  win2->end();

  embut->callback(embed_cb, win2);
  win2->callback(embed_cb, nullptr);  // "close" embeds win2 as subwindow

  win1->resizable(win1);
  win1->callback(close_cb);
  win1->size_range(200, 200);
  win1->show(argc, argv);

  win2->show();

  win3 = new Fl_Window(200, 300, "win3 - borderless");
  win3->box(FL_DOWN_BOX);
  win3->color(0xccccff00);
  win3->align(FL_ALIGN_INSIDE | FL_ALIGN_CENTER);
  win3->border(0);
  win3->end();
  win3->callback(close_cb);
  win3->show();

  return Fl::run();
}
