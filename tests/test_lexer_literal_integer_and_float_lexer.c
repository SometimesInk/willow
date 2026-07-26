#include "willow/lexer/literal_integer_and_float_lexer.h"
#include <camellia/camellia.h>
#include <camellia/err/err.h>
#include <camellia/test/test.h>
#include <camellia/type/dynamic_array.h>
#include <camellia/type/string.h>
#include <stdio.h>
#include <willow/io/io.h>
#include <willow/lexer/lexer.h>
#include <willow/lexer/lexer_utils.h>
#include <willow/lexer/literal_string_lexer.h>
#include <willow/lexer/tokens.h>

wil_lexer_context_t context;

#undef CAM_TEST_RETURN
#define CAM_TEST_RETURN(v)                                                     \
  do {                                                                         \
    wil_lexer_dispose_context(&context);                                       \
    return v;                                                                  \
  } while (0)

cam_test_result_t main(void) {
  cam_test_start();

  printf("Starting test.\n");

  // Create context
  // TODO: Edge case testing for range
  cam_cptr_t source = "1234 1234.123413";
  context = wil_lexer_create_context(source);
  CAM_TEST_ASSERT_NOT_NULL(context.source);
  CAM_TEST_ASSERT_NOT_NULL(context.current);
  CAM_TEST_ASSERT_NOT_NULL(context.start);

  // Create tokens dynamic array
  CAM_TEST_ASSERT_SUCCESS(cam_type_create_dyn_arr(
      &context.tokens, sizeof(wil_lexer_token_t), WIL_LEXER_TOKEN_INI_LEN));
  CAM_TEST_ASSERT_PREV_ERR_NOT(CAM_ERR_MEM_ALLOC);

  // First one
  wil_lexer_integer_and_float_literal(&context);
  CAM_TEST_ASSERT(cam_type_len_dyn_arr(&context.tokens) == 1);

  // Checks
  wil_lexer_token_t *token = cam_type_get_dyn_arr(&context.tokens, 0);
  wil_lexer_token_dump_int(token);
  CAM_TEST_ASSERT(token->type == WIL_LEXER_TOKEN_INTEGER);
  CAM_TEST_ASSERT(token->literal.integer = 1234);
  CAM_TEST_ASSERT(token->lexeme.str == source);
  CAM_TEST_ASSERT(token->lexeme.len == 4);

  // Emulate parser
  printf("CHARAT(%c)\n", context.current[0]);
  wil_lexer_advance(&context);
  context.start = context.current;

  // Second one
  wil_lexer_integer_and_float_literal(&context);
  CAM_TEST_ASSERT(cam_type_len_dyn_arr(&context.tokens) == 2);

  // Checks
  wil_lexer_token_t *token2 = cam_type_get_dyn_arr(&context.tokens, 1);
  wil_lexer_token_dump_dec(token2);
  CAM_TEST_ASSERT(token2->type == WIL_LEXER_TOKEN_FLOAT);
  CAM_TEST_ASSERT(token2->literal.decimal = 1234.123413);
  CAM_TEST_ASSERT(token2->lexeme.str == source + 5);
  CAM_TEST_ASSERT(token2->lexeme.len == 11);

  CAM_TEST_STOP_SUCCESS();
}
