set(UNIT_TEST_SOURCES
    test_ast_pretty_print.c
    test_sys_env_var.c
    test_conf_add_kvp.c
    test_conf_dispose.c
    test_lexer_is_at_end.c
    test_lexer_advance_and_peek_and_match.c
    test_lexer_add_token_simple.c
    test_lexer_add_token_literal.c
    test_lexer_add_token.c
    test_lexer_is_digit.c
    test_lexer_is_alpha.c
    test_lexer_is_alphanumeric.c
    test_lexer_identifier.c
    test_lexer_match_i.c
    test_lexer_literal_string_lexer.c
    test_lexer_scan_tokens.c
    test_lexer_literal_integer_and_float_lexer.c)
set(INTEGRATION_TEST_SOURCES test_io_read_file.c)

foreach(test_src ${UNIT_TEST_SOURCES})
  get_filename_component(test_name ${test_src} NAME_WE)

  add_executable(${test_name} ${test_src})
  target_link_libraries(${test_name} PRIVATE willow)

  # Add flags for better warnings
  target_compile_options(${test_name} PRIVATE -fsanitize=address,undefined
                                              -fno-omit-frame-pointer -g)

  target_link_options(${test_name} PRIVATE -fsanitize=address,undefined)

  add_test(NAME ${test_name} COMMAND $<TARGET_FILE:${test_name}>)
  set_tests_properties(${test_name} PROPERTIES LABELS "unit")
endforeach()

foreach(test_src ${INTEGRATION_TEST_SOURCES})
  get_filename_component(test_name ${test_src} NAME_WE)

  add_executable(${test_name} ${test_src})
  target_link_libraries(${test_name} PRIVATE willow)

  # Add flags for better warnings
  target_compile_options(${test_name} PRIVATE -fsanitize=address,undefined
                                              -fno-omit-frame-pointer -g)

  target_link_options(${test_name} PRIVATE -fsanitize=address,undefined)

  add_test(NAME ${test_name} COMMAND $<TARGET_FILE:${test_name}>)
  set_tests_properties(${test_name} PROPERTIES LABELS "integration")
endforeach()
