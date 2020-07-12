#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>


#include <string>


class CustomWindow : public Fl_Window {
public:
  CustomWindow(int w, int h, const char *title);
  int handle(int event) override;
  void layout();

private:
  static void buttonCallback(Fl_Button *obj, void *data);
  int _mouseGrabXOffset, _mouseGrabYOffset;
  bool _mouseGrabbing;
  int _WINDOW_W;
  int _WINDOW_H;
};

CustomWindow::CustomWindow(int w, int h, const char *title)
  : _mouseGrabXOffset(0)
  , _mouseGrabYOffset(0)
  , _mouseGrabbing(0)
  , _WINDOW_W(w)
  , _WINDOW_H(h)
  , Fl_Window(w, h, title) {}


void CustomWindow::buttonCallback(Fl_Button *obj, void *data) {
  CustomWindow *ptr = (CustomWindow *)obj->parent()->user_data();
  std::string name = (char *)data;
  if (name == "close_button") {
    exit(0); // normally we would do some cleanup, but, whatever, testcase
  }
}

int CustomWindow::handle(int event) {
  if (Fl_Group::handle(event))
    return 1;
  switch (event) {
    case FL_DRAG:
      position(Fl::event_x_root() - _mouseGrabXOffset, Fl::event_y_root() - _mouseGrabYOffset);
      return 1;
    case FL_PUSH:
      if (!_mouseGrabbing) {
	_mouseGrabbing = 1;
	_mouseGrabXOffset = Fl::event_x();
	_mouseGrabYOffset = Fl::event_y();
      }
      return 1;
    case FL_RELEASE:
      _mouseGrabbing = 0;
      return 1;
  }
  return 0;
}

void CustomWindow::layout() {
  begin();
  clear_border();  // this actually hides a window from the taskbar in X11 and Windows both pre-patch and post-patch
  skip_taskbar(0); // NEW - Allows the programmer to override said behavior
  Fl_Box *window_handle_box = new Fl_Box(FL_BORDER_BOX, 0, 0, _WINDOW_W, 16, "skip_taskbar(0) test");
  Fl_Box *textbox = new Fl_Box(FL_NO_BOX, 4, 32, _WINDOW_W, 16, "Click and drag me anywhere!");
  window_handle_box->labelcolor(fl_rgb_color(177, 100, 2));
  textbox->labelcolor(fl_rgb_color(255, 255, 255));
  window_handle_box->color(fl_rgb_color(26, 13, 13));
  color(FL_DARK3);
  user_data((void *)(this));
  Fl_Button *button = new Fl_Button(4, _WINDOW_H - 28, 96, 24, "Close");
  button->callback((Fl_Callback *)buttonCallback, (void *)"close_button");
  end();
}

int main() {
  CustomWindow window(240, 320, "Borderless Fl_Window Demo");
  window.layout();
  window.show();
  Fl::run();
  return 0;
}
