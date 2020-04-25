#------------------------------------------------------------------------------#
# Distributed under the OSI-approved Apache License, Version 2.0.  See
# accompanying file Copyright.txt for details.
#------------------------------------------------------------------------------#
#
# Findpugixml
# -----------
#
# Try to find the pugixml library
#
# This module defines the following variables:
#
#   pugixml_FOUND        - System has pugixml
#   pugixml_INCLUDE_DIRS - The pugixml include directory
#   pugixml_LIBRARIES    - Link these to use pugixml
#
# and the following imported targets:
#   pugixml::pugixml - The pugixml library target
#
# You can also set the following variable to help guide the search:
#   pugixml_ROOT - The install prefix for pugixml containing the
#              include and lib folders
#              Note: this can be set as a CMake variable or an
#                    environment variable.  If specified as a CMake
#                    variable, it will override any setting specified
#                    as an environment variable.

if(NOT pugixml_FOUND)
  if((NOT pugixml_ROOT) AND (NOT (ENV{pugixml_ROOT} STREQUAL "")))
    set(pugixml_ROOT "$ENV{pugixml_ROOT}")
  endif()
  if(pugixml_ROOT)
    set(pugixml_INCLUDE_OPTS HINTS ${pugixml_ROOT}/include NO_DEFAULT_PATHS)
    set(pugixml_LIBRARY_OPTS
      HINTS ${pugixml_ROOT}/lib ${pugixml_ROOT}/lib64
      NO_DEFAULT_PATHS
    )
  endif()

  find_path(pugixml_INCLUDE_DIR pugixml.hpp PATH_SUFFIXES pugixml ${pugixml_INCLUDE_OPTS})
  find_library(pugixml_LIBRARY NAMES pugixml ${pugixml_LIBRARY_OPTS})

  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(pugixml
    FOUND_VAR pugixml_FOUND
    REQUIRED_VARS pugixml_LIBRARY pugixml_INCLUDE_DIR
  )
  
  if(pugixml_FOUND AND NOT TARGET pugixml::pugixml)
    add_library(pugixml::pugixml UNKNOWN IMPORTED)
    set_target_properties(pugixml::pugixml PROPERTIES
      IMPORTED_LOCATION             "${pugixml_LIBRARY}"
      INTERFACE_INCLUDE_DIRECTORIES "${pugixml_INCLUDE_DIR}"
    )
    message("PUGI FOUND: ${pugixml_LIBRARY} ${pugixml_INCLUDE_DIR}")
  endif()

endif()
