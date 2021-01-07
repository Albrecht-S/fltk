//
// Data Container header file for the Fast Light Tool Kit (FLTK).
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

#ifndef _fl_data_container_h_
#define _fl_data_container_h_

#include <stdlib.h>

class Fl_Cn {

  void *data_;
  size_t data_size_;
  size_t size_;
  size_t capacity_;

public:
  size_t size() const { return size_; }

protected:
  ~Fl_Cn();
  Fl_Cn(size_t datasize = 0);
  void push(void *data);
  void *pop(void *dest);
  void *back(void *dest);
  void *at(size_t pos, void *dest);
};

class Fl_Int_Cn : public Fl_Cn {
public:
  ~Fl_Int_Cn();
  Fl_Int_Cn();
  void push(long val);
  long pop();
  long back();
  long at(size_t pos);
};

class Fl_String_Cn : public Fl_Cn {
public:
  Fl_String_Cn();
  ~Fl_String_Cn();
  void push(const char *s_);
  char *pop();
  char *back();
  char *at(size_t pos_);
};

class Fl_Ptr_Cn : public Fl_Cn {
public:
  Fl_Ptr_Cn();
  ~Fl_Ptr_Cn();
  void push(const void *p_);
  void *pop();
  void *back();
  void *at(size_t pos_);
};

#endif // _fl_data_container_h_
