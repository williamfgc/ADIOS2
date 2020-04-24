message_start_thirdparty()

if(NOT ADIOS2_USE_EXTERNAL_YAML-CPP)
  add_subdirectory(yaml-cpp)
else()
  find_package(yaml-cpp REQUIRED)
  message("    Using External yaml-cpp package")
endif()

message_end_thirdparty()
