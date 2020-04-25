message_start_thirdparty()

if(NOT ADIOS2_USE_EXTERNAL_PUGIXML)
  add_subdirectory(pugixml)
else()
  # Need a FindPugiXML.cmake, defective on Ubuntu 18
  find_package(pugixml REQUIRED MODULE)
  message("    Using External pugixml package")
endif()

message_end_thirdparty()
