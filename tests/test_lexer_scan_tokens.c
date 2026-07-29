#define CAM_TEST

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
  // Create context
  cam_cptr_t source = "int BASE = 16 - 6;";
  context = wil_lexer_create_context(source);
  CAM_TEST_ASSERT_NOT_NULL(context.source);
  CAM_TEST_ASSERT_NOT_NULL(context.current);
  CAM_TEST_ASSERT_NOT_NULL(context.start);

  // Create tokens dynamic array
  CAM_TEST_ASSERT_SUCCESS(cam_type_create_dyn_arr(
      &context.tokens, sizeof(wil_lexer_token_t), WIL_LEXER_TOKEN_INI_LEN));
  CAM_TEST_ASSERT_PREV_ERR_NOT(CAM_ERR_MEM_ALLOC);

  wil_lexer_scan_tokens(&context, &context.tokens);

  // Expecting tokens to be: WIL_LEXER_TOKEN_WM_INT ...
  // WIL_LEXER_TOKEN_IDENTIFIER ... WIL_LEXER_TOKEN_EQUAL ...
  // WIL_LEXER_TOKEN_INTEGER ... WIL_LEXER_TOKEN_MINUS ...
  // WIL_LEXER_TOKEN_INTEGER ... WIL_LEXER_TOKEN_SEMICOLON ...
  // WIL_LEXER_TOKEN_EOF, thusly 8 tokens.
  CAM_TEST_ASSERT(cam_type_len_dyn_arr(&context.tokens) == 8);

  for (int i = 0; i < cam_type_len_dyn_arr(&context.tokens); i++) {
    wil_lexer_token_t *token =
        (wil_lexer_token_t *)cam_type_get_dyn_arr(&context.tokens, i);
    printf("At i=%d: {type=%d,}\n", i, token->type);
  }

  CAM_TEST_STOP_SUCCESS();
}
