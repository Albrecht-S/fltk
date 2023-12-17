// Original version from Microsoft docs "UISettings Class":
// https://learn.microsoft.com/en-us/uwp/api/windows.ui.viewmanagement.uisettings

// 'winrt' needs to be linked with 'WindowsApp.dll' (.lib)

// Note: an alternative way to figure this out is to read the registry
// https://stackoverflow.com/questions/53501268/win10-dark-theme-how-to-use-in-winapi


#include <winrt/Windows.UI.ViewManagement.h>
using namespace winrt::Windows::UI::ViewManagement;

// Returns 1 if using dark mode and 0 if using light mode
// Prints system background and foreground colors only once.

int get_ui_color() {

  UISettings settings;
  auto background = settings.GetColorValue(UIColorType::Background);
  auto foreground = settings.GetColorValue(UIColorType::Foreground);
  static int first = 1;

  if (first) {
    printf("Background(R, G, B) = (%3d, %3d, %3d)\n", background.R, background.G, background.B);
    printf("Foreground(R, G, B) = (%3d, %3d, %3d)\n", foreground.R, foreground.G, foreground.B);
    fflush(stdout);
    first = 0;
  }

  if ((background.R + background.G + background.B) / 3 < 100) // not really a *good* test !
    return 1;
  else
    return 0;
}
