#include <camellia/camellia.h>
#include <camellia/err/err.h>
#include <camellia/test/test.h>
#include <camellia/type/dynamic_array.h>
#include <camellia/type/string.h>
#include <stdio.h>
#include <string.h>
#include <willow/io/io.h>
#include <willow/lexer/lexer.h>
#include <willow/lexer/lexer_utils.h>
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
  cam_cptr_t source = "return 1;";
  context = wil_lexer_create_context(source);
  CAM_TEST_ASSERT_NOT_NULL(context.source);
  CAM_TEST_ASSERT_NOT_NULL(context.current);
  CAM_TEST_ASSERT_NOT_NULL(context.start);

  // Create tokens dynamic array
  CAM_TEST_ASSERT_SUCCESS(cam_type_create_dyn_arr(
      &context.tokens, sizeof(wil_lexer_token_t), WIL_LEXER_TOKEN_INI_LEN));
  CAM_TEST_ASSERT_PREV_ERR_NOT(CAM_ERR_MEM_ALLOC);

  // Advance
  cam_size_t i = 0;
  while (i < strlen(source)) {
    if (wil_lexer_match_i(&context, ' '))
      break;
  }

  // Add token
  CAM_TEST_ASSERT_SUCCESS(
      wil_lexer_add_token(&context, WIL_LEXER_TOKEN_RETURN));
  CAM_TEST_ASSERT(cam_type_len_dyn_arr(&context.tokens) == 1);
  wil_lexer_token_t *token = cam_type_get_dyn_arr(&context.tokens, 0);

  // Checks
  CAM_TEST_ASSERT_NOT_NULL(token);
  CAM_TEST_ASSERT(token->type == WIL_LEXER_TOKEN_RETURN);
  CAM_TEST_ASSERT(token->lexeme.len = 6);
  CAM_TEST_ASSERT_SUCCESS(cam_type_str_match(token->lexeme.str, "return", 6));

  CAM_TEST_STOP_SUCCESS();
}
