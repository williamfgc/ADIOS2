#------------------------------------------------------------------------------#
# Distributed under the OSI-approved Apache License, Version 2.0.  See
# accompanying file Copyright.txt for details.
#------------------------------------------------------------------------------#

add_executable(TestSstWrite TestSstWrite.cpp)
add_executable(TestSstRead TestSstRead.cpp)
if(ADIOS2_HAVE_Fortran)
  add_library(TestData_f OBJECT TestData_mod.f90)
  add_executable(TestSstWrite_f TestSstWriteF.f90 $<TARGET_OBJECTS:TestData_f>)
  target_link_libraries(TestSstWrite_f adios2_f)
  add_executable(TestSstRead_f TestSstReadF.f90 $<TARGET_OBJECTS:TestData_f>)
  target_link_libraries(TestSstRead_f adios2_f)
endif()

# Workaround for multiple versions of FindSst
if(SST_INCLUDE_DIRS)
  target_include_directories(TestSstWrite PRIVATE ${SST_INCLUDE_DIRS})
  target_include_directories(TestSstRead PRIVATE ${SST_INCLUDE_DIRS})
endif()
target_link_libraries(TestSstWrite adios2 gtest_interface ${Sst_LIBRARY})
target_link_libraries(TestSstRead adios2 gtest_interface ${Sst_LIBRARY})

if(ADIOS2_HAVE_MPI)
  target_link_libraries(TestSstWrite MPI::MPI_C)
  target_link_libraries(TestSstRead MPI::MPI_C)
  set(extra_test_args EXEC_WRAPPER ${MPIEXEC_COMMAND})
  if(ADIOS2_HAVE_Fortran)
    target_link_libraries(TestSstWrite_f MPI::MPI_Fortran)
    target_link_libraries(TestSstRead_f MPI::MPI_Fortran)
  endif()
endif()

configure_file(
  run_staging_test.in
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/run_staging_test
  @ONLY
)

add_test(
  NAME ADIOSSstTest.Simple_1x1
  COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/run_staging_test
    -nw 1 -nr 1 -v -p TestSst)
set_tests_properties(ADIOSSstTest.Simple_1x1 PROPERTIES TIMEOUT 30) 

add_test(
  NAME ADIOSSstTest.2x1
  COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/run_staging_test
    -nw 2 -nr 1 -v -p TestSst)
set_tests_properties(ADIOSSstTest.2x1 PROPERTIES TIMEOUT 30) 

add_test(
  NAME ADIOSSstTest.1x2
  COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/run_staging_test
    -nw 1 -nr 2 -v -p TestSst)
set_tests_properties(ADIOSSstTest.1x2 PROPERTIES TIMEOUT 30) 

add_test(
  NAME ADIOSSstTest.3x5
  COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/run_staging_test
    -nw 3 -nr 5 -v -p TestSst)
set_tests_properties(ADIOSSstTest.3x5 PROPERTIES TIMEOUT 30) 

add_test(
  NAME ADIOSSstTest.3x5BP
  COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/run_staging_test
    -nw 3 -nr 5 -v -p TestSst -arg "MarshalMethod:BP")
set_tests_properties(ADIOSSstTest.3x5BP PROPERTIES TIMEOUT 30) 

add_test(
  NAME ADIOSSstTest.5x3
  COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/run_staging_test
    -nw 5 -nr 3 -v -p TestSst)
set_tests_properties(ADIOSSstTest.5x3 PROPERTIES TIMEOUT 30) 

if(ADIOS2_HAVE_Fortran)
  add_test(
    NAME ADIOSSstTest.FtoC_1x1
    COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/run_staging_test
    -nw 1 -nr 1 -v -w TestSstWrite_f -r TestSstRead)
  set_tests_properties(ADIOSSstTest.FtoC_1x1 PROPERTIES TIMEOUT 30) 

  add_test(
    NAME ADIOSSstTest.FtoC_1x1BP
    COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/run_staging_test
    -nw 1 -nr 1 -v -w TestSstWrite_f -r TestSstRead -arg "MarshalMethod:BP")
  set_tests_properties(ADIOSSstTest.FtoC_1x1BP PROPERTIES TIMEOUT 30) 

  add_test(
    NAME ADIOSSstTest.CtoF_1x1
    COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/run_staging_test
    -nw 1 -nr 1 -v -w TestSstWrite -r TestSstRead_f)
  set_tests_properties(ADIOSSstTest.CtoF_1x1 PROPERTIES TIMEOUT 30) 

  add_test(
    NAME ADIOSSstTest.FtoF_1x1
    COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/run_staging_test
    -nw 1 -nr 1 -v -w TestSstWrite_f -r TestSstRead_f)
  set_tests_properties(ADIOSSstTest.FtoF_1x1 PROPERTIES TIMEOUT 30) 

  add_test(
    NAME ADIOSSstTest.FtoC_3x5
    COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/run_staging_test
    -nw 3 -nr 5 -v -w TestSstWrite_f -r TestSstRead)
  set_tests_properties(ADIOSSstTest.FtoC_3x5 PROPERTIES TIMEOUT 30) 

  add_test(
    NAME ADIOSSstTest.FtoC_3x5BP
    COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/run_staging_test
    -nw 3 -nr 5 -v -w TestSstWrite_f -r TestSstRead -arg "MarshalMethod:BP")
  set_tests_properties(ADIOSSstTest.FtoC_3x5BP PROPERTIES TIMEOUT 30) 
  

  add_test(
    NAME ADIOSSstTest.CtoF_3x5
    COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/run_staging_test
    -nw 3 -nr 5 -v -w TestSstWrite -r TestSstRead_f)
  set_tests_properties(ADIOSSstTest.CtoF_3x5 PROPERTIES TIMEOUT 30) 
  add_test(
    NAME ADIOSSstTest.FtoF_3x5
    COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/run_staging_test
    -nw 3 -nr 5 -v -w TestSstWrite_f -r TestSstRead_f)
  set_tests_properties(ADIOSSstTest.FtoF_3x5 PROPERTIES TIMEOUT 30) 
endif()
