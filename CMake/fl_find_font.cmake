########################################################################
#
# function fl_find_font(FONTNAME OUTVAR)
#
# Find out if a particular font is installed on the user's system
#
# usage: fl_find_font(FONTNAME OUTVAR)
#
# FONTNAME (input) is the name of the font and must be quoted
# OUTVAR (output)  is set in the scope of the caller (PARENT_SCOPE)
#        Result:   TRUE if the font is installed, false if not
#
# Example:
# fl_find_font("Noto Sans"    FONT_SANS_FOUND)
#
########################################################################

function(fl_find_font FONTNAME OUTVAR)
  execute_process(
    COMMAND fc-list : family
    OUTPUT_VARIABLE FONTS_LIST
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )

  if(FONTS_LIST MATCHES "${FONTNAME}")
    message(STATUS "Font '${FONTNAME}' is installed.")
    set(${OUTVAR} TRUE PARENT_SCOPE)
  else()
    message(STATUS "Font '${FONTNAME}' is not installed.")
    set(${OUTVAR} FALSE PARENT_SCOPE)
  endif()
endfunction(fl_find_font FONTNAME)
