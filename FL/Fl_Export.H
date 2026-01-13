/*
 * Shared Library Export Macros for the Fast Light Tool Kit (FLTK).
 *
 * Copyright 1998-2026 by Bill Spitzak and others.
 *
 * This library is free software. Distribution and use rights are outlined in
 * the file "COPYING" which should have been included with this file.  If this
 * file is missing or damaged, see the license at:
 *
 *     https://www.fltk.org/COPYING.php
 *
 * Please see the following page on how to report bugs and issues:
 *
 *     https://www.fltk.org/bugs.php
 */

#ifndef Fl_Export_H
#define Fl_Export_H

/*
 * The following declarations are used when building DLLs under Windows
 * and shared libraries (.so's) under Unix/Linux/macOS or if the user
 * (consumer of the library) requests to build/link against a shared
 * FLTK library by defining FL_DLL.
 */

#if defined(FL_DLL)

#if defined(_WIN32)     /* Windows: use __declspec(), no matter which compiler */

#ifdef FL_LIBRARY       /* building the shared library */
#define FL_EXPORT __declspec(dllexport)
#else                   /* using the shared library */
#define FL_EXPORT __declspec(dllimport)
#endif

#else                   /* all other OS's, e.g. Linux, Unix, macOS */

#if __GNUC__ >= 4
#define FL_EXPORT __attribute__((visibility("default")))
#endif

#endif                  /* _WIN32 / other OS's */

#endif /* FL_DLL */

#ifndef FL_EXPORT
#define FL_EXPORT
#endif

#endif /* !Fl_Export_H */
