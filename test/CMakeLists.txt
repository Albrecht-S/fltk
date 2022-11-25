#
# CMakeLists.txt used to build test and demo apps by the CMake build system
#
# Copyright 2004-2021 by Bill Spitzak and others.
#
# This library is free software. Distribution and use rights are outlined in
# the file "COPYING" which should have been included with this file.  If this
# file is missing or damaged, see the license at:
#
#     https://www.fltk.org/COPYING.php
#
# Please see the following page on how to report bugs and issues:
#
#     https://www.fltk.org/bugs.php
#
#######################################################################

include (../CMake/FLTK-Functions.cmake)
include (../CMake/fl_create_example.cmake)

set (EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/../bin/test)
set (TESTFILE_PATH ${CMAKE_CURRENT_BINARY_DIR}/../data)

# create data and binary directory to copy scripts and data files

file (MAKE_DIRECTORY ${EXECUTABLE_OUTPUT_PATH})
file (MAKE_DIRECTORY ${TESTFILE_PATH})

#######################################################################
# audio libs for test apps
if (WIN32)
  set (AUDIOLIBS winmm)
elseif (APPLE)
  set (AUDIOLIBS "-framework CoreAudio")
elseif (HAVE_ALSA_ASOUNDLIB_H)
  find_library(LIB_asound asound)
  if (LIB_asound)
    set (AUDIOLIBS ${LIB_asound})
  endif (LIB_asound)
  mark_as_advanced (LIB_asound)
endif (WIN32)

#######################################################################
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
)

#######################################################################

CREATE_EXAMPLE (tabs tabs.fl fltk)
