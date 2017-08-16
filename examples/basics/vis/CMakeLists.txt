#------------------------------------------------------------------------------#
# Distributed under the OSI-approved Apache License, Version 2.0.  See
# accompanying file Copyright.txt for details.
#------------------------------------------------------------------------------#

add_executable(visSimpleTemperature visSimpleTemperature.cpp)
target_link_libraries(visSimpleTemperature adios2)

if(ADIOS2_HAVE_MPI)
  find_package(MPI COMPONENTS C REQUIRED)
  target_include_directories(visSimpleTemperature PRIVATE ${MPI_C_INCLUDE_PATH})
  target_link_libraries(visSimpleTemperature ${MPI_C_LIBRARIES})
endif()
