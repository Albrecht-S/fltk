/*
 * BSD string functions for the Fast Light Tool Kit (FLTK).
 *
 * Copyright 1998-2021 by Bill Spitzak and others.
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

#include "flstring.h"
#include <string.h>

/** \file src/flstring.c

  String handling functions.
*/

/** Safely concatenate two strings.

  This function is a replacement for the BSD function strlcat() (which is not
  available on all supported platforms) with some minor differences. It is
  compatible with strlcat() except for buffer overflow handling (see below).

  The source and destination strings \b must be NUL-terminated strings.
  The destination buffer may be empty, i.e. only contain a NUL-byte.

  The function appends the source string \p src to the destination string \p dst
  if there is room. The string is truncated if there's not enough room for the
  entire string.

  Contrary to the BSD function strlcat() there is no overflow indication. If the
  concatenated string would overflow the buffer, the result string (buffer) will
  contain only \p size - 1 bytes and the return value will be at most \p size - 1.

  \note In the special case that the input string is not NUL-terminated (which
    would be a programming error), fl_strlcat() returns \p size and the buffer
    is not modified (no NUL-termination). The return value \p size indicates
    that the string in the buffer was and is not NUL-terminated. In this error
    case this function will access memory beyond the limits of \p dst + (plus)
    \p size because strlen() is used to determine the \p dst length.

  This function never overflows the buffer length given by \p size.

  \param[inout] dst     destination string buffer (modified)
  \param[in]    src     source string
  \param[in]    size    destination buffer size

  \returns    Length of concatenated string
*/

size_t                          /* O - Length of string */
fl_strlcat(char       *dst,     /* O - Destination string buffer */
           const char *src,     /* I - Source string */
           size_t     size) {   /* I - Size of destination string buffer */
  size_t        srclen;         /* Length of source string */
  size_t        dstlen;         /* Length of destination string */

 /*
  * Figure out how much room is left...
  * Caution: take care of arithmetic with unsigned variables!
  */

  dstlen = strlen(dst);
  if (dstlen + 1 >= size)
    return (dstlen);            /* No room, return immediately... */

  size -= dstlen + 1;

 /*
  * Figure out how much room is needed...
  */

  srclen = strlen(src);

 /*
  * Copy the appropriate amount...
  */

  if (srclen > size) srclen = size;

  memcpy(dst + dstlen, src, srclen);
  dst[dstlen + srclen] = '\0';

  return (dstlen + srclen);
}


/** Safely copy a string.

  This function is a replacement for the BSD function strlcpy() (which is not
  available on all supported platforms) with some minor differences. It is
  compatible with strlcpy().

  The source string \p src \b must be a NUL-terminated string.

  The function copies the source string \p src to the destination buffer \p dst.
  The string is truncated if there's not enough room for the entire string.

  Contrary to the BSD function strlcpy() there is no overflow indication. If the
  copied string would overflow the buffer, the result string (buffer) will
  contain only \p size - 1 bytes and the return value will be at most \p size - 1.

  This function never overflows the buffer length given by \p size and the
  string in the destination buffer is always NUL-terminated.

  If the destination buffer \p size is zero, the return value is zero and
  nothing is copied.

  \param[inout] dst     destination string buffer (modified)
  \param[in]    src     source string
  \param[in]    size    destination buffer size

  \returns    Length of copied string
*/

size_t                          /* O - Length of string */
fl_strlcpy(char       *dst,     /* O - Destination string */
           const char *src,     /* I - Source string */
           size_t      size) {  /* I - Size of destination string buffer */

  size_t srclen;                /* Length of source string */

 /*
  * Figure out how much room is needed...
  */

  if (size == 0)                /* no room in buffer */
    return 0;
  size--;
  srclen = strlen(src);

 /*
  * Copy the appropriate amount...
  */

  if (srclen > size) srclen = size;
  if (srclen > 0) {
    memcpy(dst, src, srclen);
  }
  dst[srclen] = '\0';
  return (srclen);
}


#define C_RANGE(c,l,r) ( (c) >= (l) && (c) <= (r) )

/**
 * Locale independent ASCII oriented case compare function.
 *
 * returns 0 if strings successfully compare, -1 if s < t, +1 if s > t.
 */
int fl_ascii_strcasecmp(const char *s, const char *t) {
  if (!s || !t) return (s==t ? 0 : (!s ? -1 : +1));

  for(;*s && *t; s++,t++) {
    if (*s == *t) continue;
    if (*s < *t) {
      if ( (*s+0x20)!=*t || !C_RANGE(*s,'A','Z') ) return -1;
    } else {    /* (*s > *t) */
      if ( (*s-0x20)!=*t || !C_RANGE(*s,'a','z') ) return +1;
    }
  }
  return (*s==*t) ? 0 : (*t ? -1 : +1);
}
