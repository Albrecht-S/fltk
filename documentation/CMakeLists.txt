#
# CMakeLists.txt to build docs for the FLTK project using CMake (www.cmake.org)
#
# Copyright 1998-2025 by Bill Spitzak and others.
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

set(DOCS)
set(GENERATE_DOCS FALSE)
set(YEAR "")
set(CURRENT_DATE "")

if(FLTK_BUILD_HTML_DOCS OR FLTK_BUILD_PDF_DOCS)
  set(GENERATE_DOCS TRUE)
endif()

if(FLTK_INCLUDE_DRIVER_DOCS)
  set(DRIVER_DOCS "DriverDev")
else()
  set(DRIVER_DOCS "")
endif()

#------------------------------------------------
# generate files used for both HTML and PDF docs
#------------------------------------------------

if(GENERATE_DOCS)

  # create required variables

  string(TIMESTAMP YEAR "%Y" UTC)
  # note: current locale is used for abbreviated month
  string(TIMESTAMP CURRENT_DATE "%b %d, %Y" UTC)
  string(TIMESTAMP PDF_DATE "D:%Y%m%d%H%M%SZ" UTC)
  string(TIMESTAMP TODAY "%B %d, %Y" UTC)
  string(REPLACE " 0" " " TODAY "${TODAY}")

  # Find "short" doxygen version if it was built from Git
  # Note: this is still needed in CMake 3.15 but later CMake versions
  #       (notably 3.25) remove the Git revision from 'DOXYGEN_VERSION'.
  # Todo: Find the first "good" CMake version and remove this redundant
  #       code once we require this as our minimal version and replace the
  #       variable DOXYGEN_VERSION_SHORT with DOXYGEN_VERSION below.

  if(DOXYGEN_FOUND)
    # strip trailing git revision if doxygen was built from source
    string(REGEX REPLACE " .*$" "" DOXYGEN_VERSION_SHORT ${DOXYGEN_VERSION})
  endif(DOXYGEN_FOUND)

  # configure version.dox (includes the bare FLTK version number)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/version.dox.in
    ${CMAKE_CURRENT_BINARY_DIR}/version.dox
    @ONLY
  )

  # configure copyright.dox (includes current year)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/copyright.dox.in
    ${CMAKE_CURRENT_BINARY_DIR}/copyright.dox
    @ONLY
  )

  # configure generated.dox (includes date and versions)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/generated.dox.in
    ${CMAKE_CURRENT_BINARY_DIR}/generated.dox
    @ONLY
  )

  # determine Doxygen's LATEX_CMD == LaTeX Compiler (path).
  # This command name will be substituted in Doxyfile or Doxybook, respectively.

  set(FLTK_LATEX_CMD latex)       # Default: use 'latex' or 'pdflatex'

  # If 'lualatex' is installed (LUALATEX_COMPILER is set) and all required fonts
  # are available, then PDF generation uses 'lualatex' instead of 'latex'.

  if(LUALATEX_COMPILER)

    # check if required fonts are installed

    # set default font names
    set(MAIN_FONT    "Helvetica")
    set(SANS_FONT    "Helvetica")
    set(MONO_FONT    "Courier New")

    # set system specific font names
    if(APPLE)
      set(MAIN_FONT    "Helvetica")
      set(SANS_FONT    "Helvetica")
      set(MONO_FONT    "Andale Mono")  # or "Monaco" or "Courier New"
    elseif(UNIX)
      set(MAIN_FONT    "Noto Serif")
      set(SANS_FONT    "Noto Sans")
      set(MONO_FONT    "Noto Mono")
    else()
      # FIXME: Windows etc. ? Keep defaults ...
    endif()

    # now search for installed fonts
    # FIXME: Windows ???

    include(fl_find_font)

    fl_find_font("${MAIN_FONT}" MAIN_FONT_FOUND)
    fl_find_font("${SANS_FONT}" SANS_FONT_FOUND)
    fl_find_font("${MONO_FONT}" MONO_FONT_FOUND)

    # only if all required fonts are present: set FLTK_LATEX_CMD = "lualatex"

    if(MAIN_FONT_FOUND AND SANS_FONT_FOUND AND MONO_FONT_FOUND) # all fonts available

      set(FLTK_LATEX_CMD lualatex)

    else()

      message(STATUS "At least one required font for 'LuaLaTeX' was not found.")
      message(STATUS "Falling back to 'latex' or 'pdflatex', resp.")

      fl_debug_var(MAIN_FONT)
      fl_debug_var(MAIN_FONT_FOUND)

      fl_debug_var(SANS_FONT)
      fl_debug_var(SANS_FONT_FOUND)

      fl_debug_var(MONO_FONT)
      fl_debug_var(MONO_FONT_FOUND)

    endif() # all fonts available

  else(LUALATEX_COMPILER)

    message(STATUS "LuaLaTeX not found, falling back to 'latex' or 'pdflatex', resp.")

  endif()

  # ############################################################################
  # ----- test/debug -----
  # ############################################################################
  #
  # (1) set '_test_reproducible' to 1 to test "reproducible builds" (see below for details)
  # (2) set '_force_pdflatex'    to 1 to ignore `lualatex` and build PDF docs with `pdflatex`
  #
  # (1) Reproducible builds would use a given (creation) date for building the PDF docs and
  #     set the included metadata accordingly. To test this you can set an arbitrary time
  #     in the statements below and verify that the generated PDF includes that timestamp
  #     with tools like `pdfinfo` (Linux and/or macOS).
  #     You can edit the timestamp below, but take care to use the correct syntax.
  #
  # (2) This version of CMakeLists.txt detects if `lualatex` and all needed fonts are
  #     installed and enables PDF generation with `lualatex` if possible, otherwise it
  #     falls back to using the "old" command `latex` (internally: `pdflatex`) to build
  #     the PDF docs. Setting _force_pdflatex to 1 *enforces* the fallback.

  set(_test_reproducible 0)  # 1 = "reprocucible build" with timestamp as given below
  set(_force_pdflatex    0)  # 1 = enforce fallback to latex / pdflatex

  if(_test_reproducible)
    string(TIMESTAMP PDF_DATE "D:20251212030303Z" UTC)
    string(TIMESTAMP TODAY "December 12, 2025" UTC)
  endif()

  if(_force_pdflatex)
    set(FLTK_LATEX_CMD latex)
  endif()

  # ############################################################################
  # ----- end of test/debug section -----
  # ############################################################################


  # write LaTeX and LuaLaTeX specific setup file (used by both LaTeX compilers)

  set(latex_setup_file setup.tex)
  message(STATUS "--- generating ${CMAKE_CURRENT_BINARY_DIR}${latex_setup_file} from ${latex_setup_file}.in ---")

  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/${latex_setup_file}.in
    ${CMAKE_CURRENT_BINARY_DIR}/${latex_setup_file}
    @ONLY
  )

  unset(latex_setup_file)

  if(1) # debug
    fl_debug_var(YEAR)
    fl_debug_var(CURRENT_DATE)
    fl_debug_var(PDF_DATE)
    fl_debug_var(TODAY)
    fl_debug_var(FLTK_GIT_REVISION)
    fl_debug_var(DOXYGEN_FOUND)
    fl_debug_var(DOXYGEN_EXECUTABLE)
    fl_debug_var(DOXYGEN_VERSION)
    fl_debug_var(DOXYGEN_VERSION_SHORT)
    fl_debug_var(LATEX_COMPILER)
    fl_debug_var(PDFLATEX_COMPILER)
    fl_debug_var(LUALATEX_COMPILER)
    fl_debug_var(FLTK_LATEX_CMD)

    # required fonts for PDF generation with 'LuaLaTex'

    fl_debug_var(MAIN_FONT)
    fl_debug_var(MAIN_FONT_FOUND)

    fl_debug_var(SANS_FONT)
    fl_debug_var(SANS_FONT_FOUND)

    fl_debug_var(MONO_FONT)
    fl_debug_var(MONO_FONT_FOUND)
  endif()

endif(GENERATE_DOCS)

#--------------------------
# build html documentation
#--------------------------

if(FLTK_BUILD_HTML_DOCS)

  list(APPEND DOCS html)

  # generate Doxygen file "Doxyfile"

  set(GENERATE_HTML  YES)
  set(GENERATE_LATEX NO)
  set(LATEX_HEADER   "")
  set(DOXYFILE       "Doxyfile")
  set(LOGFILE        "${CMAKE_CURRENT_BINARY_DIR}/${DOXYFILE}_error.log")

  # configure Doxygen input file for HTML docs (Doxyfile.in)

  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
    ${CMAKE_CURRENT_BINARY_DIR}/${DOXYFILE}.in
    @ONLY
  )

  # convert Doxyfile to used doxygen version

  add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${DOXYFILE}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/convert_doxyfile
            ${DOXYGEN_EXECUTABLE}
            ${DOXYFILE}.in
            ${DOXYFILE}
            ${LOGFILE}
    BYPRODUCTS ${LOGFILE}
    COMMENT "Converting ${DOXYFILE} to doxygen version ${DOXYGEN_VERSION_SHORT}" VERBATIM
  )

  # generate HTML documentation

  add_custom_target(html
    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE}
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${DOXYFILE}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating HTML documentation" VERBATIM
  )

endif(FLTK_BUILD_HTML_DOCS)

#--------------------------
# build pdf documentation
#--------------------------

if(FLTK_BUILD_PDF_DOCS)

  list(APPEND DOCS pdf)

  # generate Doxygen input file "Doxybook"

  set(GENERATE_HTML  NO)
  set(GENERATE_LATEX YES)
  set(LATEX_HEADER   "${CMAKE_CURRENT_BINARY_DIR}/fltk-book.tex")
  set(DOXYFILE       "Doxybook")
  set(LOGFILE        "${CMAKE_CURRENT_BINARY_DIR}/${DOXYFILE}_error.log")

  # configure Doxygen input file for PDF docs (Doxybook.in)

  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
    ${CMAKE_CURRENT_BINARY_DIR}/${DOXYFILE}.in
    @ONLY
  )

  # convert Doxybook to current doxygen version

  # set DOXY_VERSION for compatibility with configure/make,
  # to be replaced in fltk-title.tex.in
  # FIXME: this can be simplified when configure/make is no longer supported

  set(DOXY_VERSION "${DOXYGEN_VERSION_SHORT}") #

  add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${DOXYFILE}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/convert_doxyfile
            ${DOXYGEN_EXECUTABLE}
            ${DOXYFILE}.in
            ${DOXYFILE}
            ${LOGFILE}
    BYPRODUCTS ${LOGFILE}
    COMMENT "Converting ${DOXYFILE} to doxygen version ${DOXYGEN_VERSION_SHORT}" VERBATIM
  )

  # generate LaTeX title fltk-title.tex and make_pdf script

  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/src/fltk-title.tex.in
    ${CMAKE_CURRENT_BINARY_DIR}/fltk-title.tex
    @ONLY
  )

  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/make_pdf.in
    ${CMAKE_CURRENT_BINARY_DIR}/make_pdf
    @ONLY
  )

  # generate fltk.pdf

  add_custom_command(
    OUTPUT  ${CMAKE_CURRENT_BINARY_DIR}/fltk.pdf
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/make_header
            ${DOXYGEN_EXECUTABLE}
            ${CMAKE_CURRENT_BINARY_DIR}/fltk-title.tex
            ${CMAKE_CURRENT_BINARY_DIR}/fltk-book.tex
    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE}
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/make_pdf
    COMMAND cp -f latex/refman.pdf fltk.pdf
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${DOXYFILE}
            ${CMAKE_CURRENT_BINARY_DIR}/fltk-title.tex
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating PDF documentation" VERBATIM
  )

  # add target 'pdf'

  add_custom_target(pdf
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/fltk.pdf
  )

endif(FLTK_BUILD_PDF_DOCS)

#----------------------------------
# add target 'docs' for all docs
#----------------------------------

if(DOCS)

  add_custom_target(docs
    DEPENDS ${DOCS}
  )

endif(DOCS)

#----------------------------------
# install html + pdf documentation
#----------------------------------

if(FLTK_INSTALL_HTML_DOCS AND FLTK_BUILD_HTML_DOCS)

  install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html
    DESTINATION ${FLTK_DATADIR}/doc/fltk
  )

endif(FLTK_INSTALL_HTML_DOCS AND FLTK_BUILD_HTML_DOCS)

if(FLTK_INSTALL_PDF_DOCS AND FLTK_BUILD_PDF_DOCS)

  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/fltk.pdf
    DESTINATION ${FLTK_DATADIR}/doc/fltk/
  )

endif(FLTK_INSTALL_PDF_DOCS AND FLTK_BUILD_PDF_DOCS)
