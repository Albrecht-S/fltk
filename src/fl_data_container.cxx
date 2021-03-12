//
// Data Container implementation for the Fast Light Tool Kit (FLTK).
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

#include "fl_data_container.h"
#include <FL/fl_string.h>

#include <stdlib.h>
#include <string.h>

Fl_Cn::Fl_Cn(size_t datasize)
  : data_(0)
  , data_size_(datasize)
  , size_(0)
  , capacity_(0) {}

Fl_Cn::~Fl_Cn() {
  free(data_);
}

void Fl_Cn::push(void *item) {
  if (!data_) {
    static const size_t INITIAL_CAPACITY = 16;
    data_ = malloc(INITIAL_CAPACITY * data_size_);
    if (data_)
      capacity_ = INITIAL_CAPACITY;
  }
  if (data_) {
    if (capacity_ <= size_) {
      size_t new_capacity = (capacity_ * 12 + 7) / 8; // old * 1.5, divisible by 8
      void *d = realloc(data_, new_capacity);
      if (!d)
        return;
      data_ = d;
      capacity_ = new_capacity;
    }
    void *d = (char *)data_ + data_size_ * size_;
    memcpy(d, item, data_size_);
    size_++;
  }
}

void *Fl_Cn::pop(void *dest) {
  if (!size_)
    return 0;
  void *d = (char *)data_ + data_size_ * (size_ - 1);
  memcpy(dest, d, data_size_);
  size_--;
  return dest;
}

void *Fl_Cn::back(void *dest) {
  if (!size_)
    return 0;
  void *d = (char *)data_ + data_size_ * (size_ - 1);
  memcpy(dest, d, data_size_);
  return dest;
}

void *Fl_Cn::at(size_t pos, void *dest) {
  if (pos >= size_)
    return 0;
  void *d = (char *)data_ + data_size_ * pos;
  memcpy(dest, d, data_size_);
  return d;
}


Fl_Int_Cn::Fl_Int_Cn()
  : Fl_Cn(sizeof(long)) {}

Fl_Int_Cn::~Fl_Int_Cn() {}

void Fl_Int_Cn::push(long val) {
  Fl_Cn::push(&val);
}

long Fl_Int_Cn::pop() {
  long n = 0;
  Fl_Cn::pop(&n);
  return n;
}

long Fl_Int_Cn::back() {
  long n = 0;
  Fl_Cn::back(&n);
  return n;
}

long Fl_Int_Cn::at(size_t pos) {
  long n = 0;
  Fl_Cn::at(pos, &n);
  return n;
}

Fl_String_Cn::Fl_String_Cn()
  : Fl_Cn(sizeof(char *)) {}

// The destructor frees all stored strings
Fl_String_Cn::~Fl_String_Cn() {
  while (size() > 0) {
    free((void *)pop());
  }
}

void Fl_String_Cn::push(const char *item) {
  char *s = fl_strdup(item);
  return Fl_Cn::push(&s);
}

char *Fl_String_Cn::pop() {
  char *s;
  Fl_Cn::pop(&s);
  return s;
}

char *Fl_String_Cn::back() {
  char *s = 0;
  Fl_Cn::back(&s);
  return s;
}

char *Fl_String_Cn::at(size_t pos) {
  char *s = 0;
  Fl_Cn::at(pos, &s);
  return s;
}


Fl_Ptr_Cn::Fl_Ptr_Cn()
  : Fl_Cn(sizeof(void *)) {}

Fl_Ptr_Cn::~Fl_Ptr_Cn() {}

void Fl_Ptr_Cn::push(const void *pt) {
  Fl_Cn::push(&pt);
}

void *Fl_Ptr_Cn::pop() {
  void *p = 0;
  Fl_Cn::pop(&p);
  return p;
}

void *Fl_Ptr_Cn::back() {
  void *p = 0;
  Fl_Cn::back(&p);
  return p;
}

void *Fl_Ptr_Cn::at(size_t pos) {
  void *p = 0;
  Fl_Cn::at(pos, &p);
  return p;
}
