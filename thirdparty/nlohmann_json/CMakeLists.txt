message_start_thirdparty()

if(NOT ADIOS2_USE_EXTERNAL_NLOHMANN_JSON)
  add_subdirectory(nlohmann_json)
else()
  # Doesn't build on Ubuntu 18, old version 2.1.1   
  find_package(nlohmann_json 3.5.0 REQUIRED)
  message("    Using External nlohmann_json package")
endif()

message_end_thirdparty()
