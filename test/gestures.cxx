#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Dial.H>

#include <FL/fl_ask.H>
#include <FL/fl_draw.H>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

const float zmin = 0.4; // min. zoom
const float zmax = 2.0; // max. zoom

void exitcb(Fl_Widget *, void *) {
  exit(0);
}

class myButton : public Fl_Button {
  char txt_[20];
  double zoom;
  int bx, by, bw, bh;

public:
  myButton(int x, int y, int w, int h): Fl_Button(x, y, w, h, "") {
    bx = x; by = y; bw = w; bh = h;
    zoom = 1.0;
    strcpy(txt_, "1.0");
    align(FL_ALIGN_CENTER | FL_ALIGN_INSIDE);
    label(txt_);
  }

  int handle(int event) {
    int ret = 0;
    double z = 0.0;
    switch(event) {
      case FL_MOUSEWHEEL:
      case FL_SCROLL_GESTURE:
        if (Fl::event_dy() > 0)
          zoom += 0.1;
        else
          zoom -= 0.1;
        if (zoom < zmin) zoom = zmin;
        if (zoom > zmax) zoom = zmax;
        sprintf(txt_, "%5.3f", zoom);
        resize(bx, by, bw * zoom, bh * zoom);
        parent()->redraw();
        ret = 1;
        break;
      case FL_ZOOM_GESTURE:
        z = Fl::event_dy()/100000.0;
        printf("handle FL_ZOOM_GESTURE: %d (%5.3f)\n", Fl::event_dy(), z);
        if (z > 0)
          zoom *= z;
        if (zoom < zmin) zoom = zmin;
        if (zoom > zmax) zoom = zmax;
        sprintf(txt_, "%5.3f", zoom);
        resize(bx, by, bw * zoom, bh * zoom);
        parent()->redraw();
        ret = 1;
        break;
      default:
        break;
    }
    return Fl_Button::handle(event) ? 1 : ret;
  }
};


class myDial : public Fl_Dial {
  char txt_[20];
  double angle;
  int bx, by, bw, bh;

public:
  myDial(int x, int y, int w, int h): Fl_Dial(x, y, w, h, "") {
    bx = x; by = y; bw = w; bh = h;
    angle = 90.0;
    strcpy(txt_, "90.0");
    align(FL_ALIGN_CENTER | FL_ALIGN_TOP);
    label(txt_);
    angles(0, 360);
    type(FL_LINE_DIAL);
    minimum(0);
    maximum(360);
    step(1);
    set_value();
  }

  void set_value() {
    value(double(int(angle + 180) % 360));
  }

  void set_label() {
    angle = (int(value()) + 180) % 360;
    sprintf(txt_, "%3.0f", angle);
    parent()->redraw();
  }

  int handle(int event) {
    int ret = 0;
    switch(event) {
      case FL_MOUSEWHEEL:
      case FL_SCROLL_GESTURE:
        if (Fl::event_dy() > 0)
          angle += 10.0;
        else
          angle -= 10.;
        if (angle < 0.) angle += 360.;
        if (angle > 360.) angle -= 360;
        sprintf(txt_, "%3.0f", angle);
        set_value();
        parent()->redraw();
        return 1;
        break;
      case FL_ROTATE_GESTURE:
        angle += Fl::event_dy() / 100000.;
        if (angle < 0.) angle += 360.;
        if (angle > 360.) angle -= 360;
        sprintf(txt_, "%3.0f", angle);
        set_value();
        parent()->redraw();
        return 1;
        break;
      default:
        break;
    }
    return Fl_Dial::handle(event) ? 1 : ret;
  }
};

void dial_cb(Fl_Widget *w, void *v) {
  myDial *d = (myDial *)w;
  d->set_label();
}

class myPad : public Fl_Group{
  float rx, ry, rscale, rtheta;

public:
  myPad(int x, int y, int w, int h, const char * l = 0 ): Fl_Group(x, y, w, h, l) {
    reset();
  }

  void reset() {
    rx = 0;
    ry = 0;
    rscale = 1;
    rtheta = 0.0;
  }

  int handle(int event) {
    int ret = 0;
    double z = 0;
    switch(event) {
//      case FL_RELEASE:
//        reset();
//        parent()->redraw();
//        return 1;
//        break;
      case FL_MOUSEWHEEL:
        if (Fl::event_dy() > 0)
          rscale += 0.1;
        else
          rscale -= 0.1;
        if (rscale < zmin) rscale = zmin;
        if (rscale > zmax) rscale = zmax;
        redraw();
        return 1;
        break;
      case FL_ZOOM_GESTURE:
        z = Fl::event_dy()/100000.0;
        printf("handle FL_ZOOM_GESTURE: %d (%5.3f)\n", Fl::event_dy(), z);
        if (z > 0)
          rscale *= z;
        if (rscale < zmin) rscale = zmin;
        if (rscale > zmax) rscale = zmax;
        redraw();
        return 1;
        break;
      case FL_SCROLL_GESTURE:
        if (Fl::event_dy() != 0) ry += Fl::event_dy();
        if ( ry > 200 ) ry = 200;
        if ( ry < -200 ) ry = -200;
        if (Fl::event_dx() != 0) rx += Fl::event_dx();
        if ( rx > 200 ) rx = 200;
        if ( rx < -200 ) rx = -200;
        redraw();
        return 1;
        break;
      case FL_ROTATE_GESTURE:
        rtheta -= Fl::event_dy() / 100000.;
        if (rtheta < 0.) rtheta += 360.;
        if (rtheta > 360.) rtheta -= 360;
        redraw();
        return 1;
        break;
      default:
        break;
    }
    return Fl_Group::handle(event) ? 1 : ret;
  }

  void draw() {
    Fl_Group::draw();

    int rw = 30;

    fl_color(FL_BLACK);

    fl_push_matrix();
    fl_translate( 880 + 220 + rx, 220 + ry ); // Center of group
    fl_rotate( rtheta );
    fl_scale( rscale );

    fl_begin_polygon();
    fl_vertex( -rw, -rw );
    fl_vertex( rw, -rw );
    fl_vertex( rw, rw );
    fl_vertex( -rw, rw );
    fl_end_polygon();

    fl_pop_matrix();

  }
};

int main(int argc, char **argv) {

  Fl_Double_Window *window = new Fl_Double_Window(3*440, 510);

  // embed widgets in their own groups to speed up drawing

  Fl_Group *g1 = new Fl_Group(0, 0, 440, 440);
  g1->box(FL_FLAT_BOX);
  g1->color(0xddffdd00); // light green
  myButton *b1 = new myButton(20, 20, 200, 100);
  myDial *d1 = new myDial(130, 240, 180, 180);
  d1->callback(dial_cb);
  g1->end();

  Fl_Group *g2 = new Fl_Group(440, 0, 440, 440);
  g2->box(FL_FLAT_BOX);
  g2->color(0xddddff00); // light blue
  myDial *d2 = new myDial(460, 30, 380, 380);
  d2->callback(dial_cb);
  g2->end();

  myPad *g3 = new myPad(880, 0, 440, 440);
  g3->box(FL_FLAT_BOX);
  g3->color(0xffdddd00); // light red
  g3->end();

  Fl_Button *b3 = new Fl_Button(10, 450, 1300, 50, "E&xit");
  b3->callback(exitcb, 0);

  window->end();
  window->resizable(window);
  window->size_range(430, 240);
  window->show(argc, argv);

  return Fl::run();
}
