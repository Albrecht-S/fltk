//
// strlcpy and strlcat test program for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2021 by Bill Spitzak and others.
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

// DO NOT include config.h to enforce calling the FLTK versions ...
// #include <config.h>

#include "../src/flstring.h"
#include <stdio.h>

void print(const char *buf, size_t size) {
  printf("size = %3d, buf = '%s'\n", int(size), buf);
  fflush(stdout);
}

int main(int argc, char **argv) {

  char buf[256];
  size_t bs = sizeof(buf);
  size_t nn = 0;

  nn = strlcpy(buf, "", bs);
  print(buf, nn);

  nn = strlcpy(buf, "This is a very very very long string!", bs);
  print(buf, nn);

  nn = strlcat(buf, " + Short string.", bs);
  print(buf, nn);

  // Exercise buffer size 0 (buffer full condition)

  nn = strlcat(buf, " + Short string.", 0);
  print(buf, nn);

  // 'buf' now contains a string longer than 50 characters. In the context
  // of strlcat(buf, str, 50) this is an "unterminated" string (buffer full).

  nn = strlcat(buf, " + Short string.", 50);
  print(buf, nn);

  // Copy a string to zero-length buffer: does NOT change the buffer

  nn = strlcpy(buf, "aaa", 0);
  print(buf, nn);

  // Clear the buffer (copy any string to buffer size 1)

  nn = strlcpy(buf, "bbb", 1);
  print(buf, nn);

  // "Append" a short string

  for (int i = 0; i < 5; i++) {
    nn = strlcat(buf, "Short string. ", 50);
    print(buf, nn);
  }

  return 0;
}
