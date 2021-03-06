set (TESTS context_test context_test2 fortran_test get_set_test no_leaf_test)
set (PROGS ffs_write ffs_index_test ffs_file_test test_file_unique)

foreach (TEST ${TESTS} )
    ADD_EXECUTABLE(${TEST} ${TEST}.c test_funcs.c)
    TARGET_LINK_LIBRARIES(${TEST} ffs)
    ADD_TEST(NAME ${TEST} COMMAND ${TEST})
endforeach()

ADD_EXECUTABLE(marshal_test marshal_test.c)
TARGET_LINK_LIBRARIES(marshal_test ffs)
ADD_TEST(NAME marshal_test COMMAND marshal_test)

ADD_EXECUTABLE(marshal_test2 marshal_test2.c)
TARGET_LINK_LIBRARIES(marshal_test2 ffs)
ADD_TEST(NAME marshal_test2 COMMAND marshal_test2)

foreach (PROG ${PROGS} )
    ADD_EXECUTABLE(${PROG} ${PROG}.c test_funcs.c)
    TARGET_LINK_LIBRARIES(${PROG} ffs)
endforeach()

if (NOT (DEFINED CercsArch))
    execute_process(COMMAND cercs_arch OUTPUT_VARIABLE CercsArch ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)
    MARK_AS_ADVANCED(CercsArch)
endif()

FILE(GLOB CTOS ${FFS_SOURCE_DIR}/ffs/output_dir/context_test_output*.bin)
FILE(GLOB FOS ${FFS_SOURCE_DIR}/ffs/output_dir/file_output*.bin)
FILE(GLOB IOS ${FFS_SOURCE_DIR}/ffs/output_dir/index_output*.bin)

ADD_TEST(NAME write_context_output
  COMMAND context_test -w ${FFS_BINARY_DIR}/ffs/tests/context_test_output.bin)
ADD_TEST(NAME context_output_unique
  COMMAND test_file_unique
    ${FFS_BINARY_DIR}/ffs/tests/context_test_output.bin ${CTOS})
ADD_TEST(NAME write_file_output
  COMMAND ffs_write ${FFS_BINARY_DIR}/ffs/tests/file_output.bin)
ADD_TEST(NAME file_output_unique
  COMMAND test_file_unique ${FFS_BINARY_DIR}/ffs/tests/file_output.bin ${FOS})
ADD_TEST(NAME write_index_output
  COMMAND ffs_write -a ${FFS_BINARY_DIR}/ffs/tests/index_output.bin)
ADD_TEST(NAME index_output_unique
  COMMAND test_file_unique ${FFS_BINARY_DIR}/ffs/tests/index_output.bin ${IOS})
ADD_TEST(NAME write_index_reopen
  COMMAND ffs_write -a -r -i ./index_reopen_output.bin)
LIST(APPEND CTOS context_test_output.bin)
LIST(REMOVE_DUPLICATES CTOS)
LIST(APPEND FOS file_output.bin)
LIST(REMOVE_DUPLICATES FOS)
LIST(APPEND IOS index_output.bin)
LIST(APPEND IOS index_reopen_output.bin)
LIST(REMOVE_DUPLICATES IOS)

foreach	(CTOUT ${CTOS})
    GET_FILENAME_COMPONENT(BASENAME ${CTOUT} NAME)
    ADD_TEST(NAME CTOUT_${BASENAME} COMMAND context_test -r ${CTOUT})
    set_property(TEST CTOUT_${BASENAME} APPEND PROPERTY DEPENDS write_context_output)
endforeach()

foreach	(FOUT ${FOS})
    GET_FILENAME_COMPONENT(BASENAME ${FOUT} NAME)
    ADD_TEST(NAME FOUT_${BASENAME} COMMAND ffs_file_test ${FOUT})
    set_property(TEST FOUT_${BASENAME} APPEND PROPERTY DEPENDS write_file_output)
endforeach()

foreach	(IOUT ${IOS})
    GET_FILENAME_COMPONENT(BASENAME ${IOUT} NAME)
    ADD_TEST(NAME IOUT_${BASENAME} COMMAND ffs_index_test ${IOUT})
    set_property(TEST IOUT_${BASENAME} APPEND PROPERTY DEPENDS write_index_output)
    set_property(TEST IOUT_${BASENAME} APPEND PROPERTY DEPENDS ffs_index_test)
endforeach()

