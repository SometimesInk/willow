#include <camellia/camellia.h>
#include <camellia/err/err.h>
#include <camellia/test/test.h>
#include <camellia/type/dynamic_array.h>
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
    if (&context.tokens != (cam_type_dyn_arr_t *)CAM_NULL) {                   \
      cam_type_free_dyn_arr(&context.tokens);                                  \
    }                                                                          \
    return v;                                                                  \
  } while (0)

cam_test_result_t main(void) {
  cam_test_start();

  printf("Starting test.\n");

  // Create context
  cam_cptr_t source = "__WIL_VERSION = \"1.0.0\"\n";
  context = wil_lexer_create_context(source);
  CAM_TEST_ASSERT_NOT_NULL(context.source);
  CAM_TEST_ASSERT_NOT_NULL(context.current);
  CAM_TEST_ASSERT_NOT_NULL(context.start);

  printf("Creating dynamic array.\n");

  // Create tokens dynamic array
  CAM_TEST_ASSERT_SUCCESS(
      cam_type_create_dyn_arr(&context.tokens, sizeof(wil_lexer_token_t), 8));
  CAM_TEST_ASSERT_PREV_ERR_NOT(CAM_ERR_MEM_ALLOC);

  printf("Created dynamic array.\n");

  // Add token
  CAM_TEST_ASSERT_SUCCESS(
      wil_lexer_add_token_simple(&context, WIL_LEXER_TOKEN_DOT));
  CAM_TEST_ASSERT(cam_type_len_dyn_arr(&context.tokens) == 1);
  wil_lexer_token_t *token = cam_type_get_dyn_arr(&context.tokens, 0);
  CAM_TEST_ASSERT_NOT_NULL(token);
  CAM_TEST_ASSERT(token->type == WIL_LEXER_TOKEN_DOT);

  CAM_TEST_STOP_SUCCESS();
}
