message_start_thirdparty()

if(NOT ADIOS2_USE_EXTERNAL_RAPIDJSON)
  add_library(RapidJSON INTERFACE)
  target_include_directories(RapidJSON INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/RapidJSON/include>
  )
  install(TARGETS RapidJSON EXPORT adios2Exports)
else()
  find_package(RapidJSON REQUIRED)
  message("    Using External RapidJSON package")  
endif()

message_end_thirdparty()
