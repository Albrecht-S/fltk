//
// Data Container test program for the Fast Light Tool Kit (FLTK).
//
// Copyright 2020-2021 by Bill Spitzak and others.
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

#include "../src/fl_data_container.h"

// #include <stdlib.h>
// #include <string.h>
#include <stdio.h>

int main() {
  int i;
  const char *pr;
  Fl_Int_Cn ii;
  ii.push(-1);
  ii.push(1);
  ii.push(-1);
  ii.push(1);
  ii.push(2);
  ii.push(3);
  printf("pushed: -1, 2, -1, 1, 2, 3, size = %lu\n", ii.size());
  i = ii.back();
  printf("back: %d, size = %lu\n", i, ii.size());
  i = ii.back();
  printf("back: %d, size = %lu\n", i, ii.size());

  i = ii.pop();
  printf("popped: %d, size = %lu\n", i, ii.size());
  i = ii.pop();
  printf("popped: %d, size = %lu\n", i, ii.size());
  i = ii.pop();
  printf("popped: %d, size = %lu\n", i, ii.size());

  Fl_String_Cn ss;
  ss.push("abc");
  char *s = ss.pop();
  printf("%s\n", s);
  free(s);

  Fl_Ptr_Cn dd;
  const char *p = "abcdef";
  dd.push(p);
  dd.push("alpha-beta");
  dd.push("xyz");
  while (dd.size() > 0) {
    int n = dd.size();
    pr = (const char *)dd.pop();
    printf("size = %d, popped = %s\n", n, pr);
  }

  Fl_String_Cn *cs = new Fl_String_Cn();

  cs->push("ab");
  cs->push("abcd");
  cs->push("abcdefg");
  cs->push("xyzasjhfaew.fyknds");
  cs->push("qrquzoiqwzqiztitzitzitzqitziozrtoiret");
  pr = (const char *)cs->pop();
  printf("size = %d, popped = %s\n", (int)cs->size(), pr);
  free((void *)pr);

  delete cs;
}
