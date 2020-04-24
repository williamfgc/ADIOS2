#------------------------------------------------------------------------------#
# Distributed under the OSI-approved Apache License, Version 2.0.  See
# accompanying file Copyright.txt for details.
#------------------------------------------------------------------------------#
#
# FindRapidJSON
# -----------
#
# Try to find the RapidJSON library
#
# This module defines the following variables:
#
#   RapidJSON_FOUND        - System has RapidJSON
#   RapidJSON_INCLUDE_DIRS - The RapidJSON include directory
#
# and the following imported targets:
#   RapidJSON::RapidJSON - The RapidJSON compression library target
#
# You can also set the following variable to help guide the search:
#   RapidJSON_ROOT - The install prefix for RapidJSON containing the
#              include and lib folders
#              Note: this can be set as a CMake variable or an
#                    environment variable.  If specified as a CMake
#                    variable, it will override any setting specified
#                    as an environment variable.

if(NOT RapidJSON_FOUND)
  if((NOT RapidJSON_ROOT) AND (NOT (ENV{RapidJSON_ROOT} STREQUAL "")))
    set(RapidJSON_ROOT "$ENV{RapidJSON_ROOT}")
  endif()
  if(RapidJSON_ROOT)
    set(RapidJSON_INCLUDE_OPTS HINTS ${RapidJSON_ROOT}/include NO_DEFAULT_PATHS)
  endif()

  find_path(RapidJSON_INCLUDE_DIR rapidjson.h PATH_SUFFIXES rapidjson ${RapidJSON_INCLUDE_OPTS})

  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(RapidJSON
    FOUND_VAR RapidJSON_FOUND
    REQUIRED_VARS RapidJSON_INCLUDE_DIR
  )
  if(RapidJSON_FOUND)
    set(RapidJSON_INCLUDE_DIRS ${RapidJSON_INCLUDE_DIR})
    if(RapidJSON_FOUND AND NOT TARGET RapidJSON::RapidJSON)
      add_library(RapidJSON::RapidJSON UNKNOWN IMPORTED)
      set_target_properties(RapidJSON::RapidJSON PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${RapidJSON_INCLUDE_DIR}"
      )
    endif()
  endif()
endif()
