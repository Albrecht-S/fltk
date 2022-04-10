//
// Windows touch gesture support code for the Fast Light Tool Kit (FLTK).
//
// Copyright 2021-2022 by Bill Spitzak and others.
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

// This file contains Windows specific code to support multi-touch gestures.

#include <FL/Fl.H>
#include <FL/Fl_Window.H>

#if defined(_WIN32) && !defined(FL_DOXYGEN)

// We require Windows 7 features to support touch gestures
#define FL_WINVER_MIN 0x0601 // Windows 7 for touch gestures

#if !defined(WINVER) || (WINVER < FL_WINVER_MIN)
#ifdef WINVER
#undef WINVER
#endif
#define WINVER FL_WINVER_MIN
#endif
#if !defined(_WIN32_WINNT) || (_WIN32_WINNT < FL_WINVER_MIN)
#ifdef _WIN32_WINNT
#undef _WIN32_WINNT
#endif
#define _WIN32_WINNT FL_WINVER_MIN
#endif

#include <windows.h>

// Debug flag:  0 = off, 1 = decode, 2 = zoom, 4 = pan, 8 = rotate,
//              16 = tow-finger tap, 32 = press and tap, 64 = unknown,
//              0xff = everything
#define DEBUG_TOUCH 0xff

#if DEBUG_TOUCH
#include <stdio.h> // DEBUG: printf()
#endif

/*
  https://docs.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-gestureinfo

  The following table indicates the various identifiers for gestures.

    Name               Value  Description
    GID_BEGIN            1    A gesture is starting.
    GID_END              2    A gesture is ending.
    GID_ZOOM             3    The zoom gesture.
    GID_PAN              4    The pan gesture.
    GID_ROTATE           5    The rotation gesture.
    GID_TWOFINGERTAP     6    The two-finger tap gesture.
    GID_PRESSANDTAP      7    The press and tap gesture.
*/
#if (DEBUG_TOUCH & 1)
static const char *gestures[] = {
  "unknown gesture", "GID_BEGIN",  "GID_END",          "GID_ZOOM",
  "GID_PAN",         "GID_ROTATE", "GID_TWOFINGERTAP", "GID_PRESSANDTAP"
};
#endif // (DEBUG_TOUCH & 1)

// This function is used to select parts or all touch gesture messages.
// Currently all messages are enabled

int fl_win32_SetGestureConfig(HWND hWnd) {
  GESTURECONFIG gc = {0, GC_ALLGESTURES, 0};
  BOOL bResult = SetGestureConfig(hWnd, 0, 1, &gc, sizeof(GESTURECONFIG));

  if (!bResult) {
#if DEBUG_TOUCH
    DWORD dwErr = GetLastError();
    printf("Error calling SetGestureConfig(), dwErr = %ld.\n", dwErr);
    fflush(stdout);
#endif // DEBUG_TOUCH
    return 0;
  }
  return 1;

} // fl_win32_SetGestureConfig()

// This function does all the system specific work to decode a gesture message.
// It returns 1 if the gesture could be decoded and should be handled by FLTK,
// zero (0) otherwise.

int fl_win32_DecodeGesture(Fl_Window *window, LPARAM lParam) {
  // Create and populate a structure to retrieve the extra message info.
  GESTUREINFO gi;
  ZeroMemory(&gi, sizeof(GESTUREINFO));
  gi.cbSize = sizeof(GESTUREINFO);

  // Retrieve the extra message info.
  BOOL bResult = GetGestureInfo((HGESTUREINFO)lParam, &gi);

  int handled = 0;

  if (bResult) {
    // now interpret the gesture
#if (DEBUG_TOUCH & 1)
    const char *name = gestures[0];
    if (gi.dwID < 8)
      name = gestures[gi.dwID];
    printf("Gesture %-16s (%lu): flags=%02x, args=%lu, pos=(%4d,%4d)\n",
           name, gi.dwID, (int)gi.dwFlags,
           (unsigned long)gi.ullArguments, gi.ptsLocation.x, gi.ptsLocation.y);
#endif // DEBUG_TOUCH

    double angle = 0.0;
    double degrees = 0.0;
    static int begin = -1;
    static long zoom_init = 0;
    static long zoom_distance = 0;
    static double zoom_factor = 0.0;
    static double zoom_total  = 0.0;
    int pan_x = 0;
    int pan_y = 0;
    int pan_distance = 0;

    switch (gi.dwID) {
      case GID_BEGIN:
        begin = 1;
        zoom_init = 0;
        zoom_distance = 0;
        break;
      case GID_END:
        begin = -1;
        break;
      case GID_ZOOM:
        // Code for zooming goes here
        if (begin == 1) {
          zoom_init = (long)gi.ullArguments;
          zoom_distance = zoom_init;
          zoom_factor = 1.0;
          zoom_total = 1.0;
          begin = 0;
        } else {
          long new_distance = (long)gi.ullArguments;
          if (new_distance != zoom_distance) {
            zoom_factor = (double)new_distance / zoom_distance;
            zoom_total  = zoom_total * zoom_factor;
            zoom_distance = new_distance;
#if (DEBUG_TOUCH & 2)
          printf("    zoom init = %8ld, distance = %8ld,               factor = %8.6f, total = %8.6f\n",
                zoom_init, zoom_distance, zoom_factor, zoom_total);
#endif // (DEBUG_TOUCH & 2)
          } else {
            zoom_factor = 1.0;
          }
        }
        Fl::e_value = zoom_factor;                    // since FLTK 1.4.0
        Fl::e_dy = (zoom_factor - 1.0) * 1000;        // FLTK 1.3.x backwards compatibiity (macOS)
        printf("***** Fl::handle(FL_ZOOM_GESTURE) f = %5.3f, e_dy = %6d\n", Fl::e_value, Fl::e_dy);
        Fl::handle(FL_ZOOM_GESTURE, window);
        handled = 1;
        break;
      case GID_PAN:
        // Code for panning goes here
        pan_x = gi.ptsLocation.x;
        pan_y = gi.ptsLocation.y;
        pan_distance = (int)gi.ullArguments;
#if (DEBUG_TOUCH & 4)
          printf("    pan gesture at (%4d, %4d), distance = %d\n", pan_x, pan_y, pan_distance);
#endif // (DEBUG_TOUCH & 4)
        handled = 1;
        break;
      case GID_ROTATE:
        // Code for rotation goes here
        angle = GID_ROTATE_ANGLE_FROM_ARGUMENT(gi.ullArguments);
        degrees = angle / (2 * 3.1415926535) * 360;
#if (DEBUG_TOUCH & 8)
        printf("    rotation angle = %7.2f (%7.2f degrees)\n", angle, degrees);
#endif // (DEBUG_TOUCH & 8)
        handled = 1;
        break;
      case GID_TWOFINGERTAP:
        // Code for two-finger tap goes here
#if (DEBUG_TOUCH & 16)
        printf("    two finger tap\n");
#endif // (DEBUG_TOUCH & 16)
        break;
      case GID_PRESSANDTAP:
        // Code for roll over goes here
#if (DEBUG_TOUCH & 32)
        printf("    press and tap (aka roll over)\n");
#endif // (DEBUG_TOUCH & 32)
        break;
      default:
        // unknown gesture
#if (DEBUG_TOUCH & 64)
        printf("    unknown gesture (%ld)\n", (long)gi.dwID);
#endif // (DEBUG_TOUCH & 64)
        break;
    }
  } else {
#if DEBUG_TOUCH
    DWORD dwErr = GetLastError();
    if (dwErr > 0) {
      printf("Could not retrieve a GESTUREINFO structure, dwErr = %ld.\n", dwErr);
#endif // DEBUG_TOUCH
    }
  }
#if DEBUG_TOUCH
  fflush(stdout);
#endif // DEBUG_TOUCH
  return handled;

} // DecodeGesture()

#endif // defined(_WIN32) && !defined(FL_DOXYGEN)
