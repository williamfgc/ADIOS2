message_start_thirdparty()

if(NOT ADIOS2_USE_EXTERNAL_PYBIND11)
  add_subdirectory(pybind11)
else()
  # Doesn't build on Ubuntu 18, old version 2.0.1
  find_package(pybind11 2.4.3 REQUIRED)
  message("    Found External pybind11 package")
endif()

message_end_thirdparty()
