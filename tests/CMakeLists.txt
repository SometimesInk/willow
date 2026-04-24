set(UNIT_TEST_SOURCES test_sys_env_var.c test_conf_add_kvp.c
                      test_conf_dispose.c)
set(INTEGRATION_TEST_SOURCES test_io_read_file.c)

foreach(test_src ${UNIT_TEST_SOURCES})
  get_filename_component(test_name ${test_src} NAME_WE)

  add_executable(${test_name} ${test_src})
  target_link_libraries(${test_name} PRIVATE willow)

  add_test(NAME ${test_name} COMMAND $<TARGET_FILE:${test_name}>)
  set_tests_properties(${test_name} PROPERTIES LABELS "unit")
endforeach()

foreach(test_src ${INTEGRATION_TEST_SOURCES})
  get_filename_component(test_name ${test_src} NAME_WE)

  add_executable(${test_name} ${test_src})
  target_link_libraries(${test_name} PRIVATE willow)

  add_test(NAME ${test_name} COMMAND $<TARGET_FILE:${test_name}>)
  set_tests_properties(${test_name} PROPERTIES LABELS "integration")
endforeach()
