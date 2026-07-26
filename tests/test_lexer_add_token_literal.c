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
    wil_lexer_dispose_context(&context);                                       \
    return v;                                                                  \
  } while (0)

cam_test_result_t main(void) {
  cam_test_start();

  printf("Starting test.\n");

  // Create context
  cam_cptr_t source = "10";
  context = wil_lexer_create_context(source);
  CAM_TEST_ASSERT_NOT_NULL(context.source);
  CAM_TEST_ASSERT_NOT_NULL(context.current);
  CAM_TEST_ASSERT_NOT_NULL(context.start);
  context.current += 2; // On \0

  printf("Creating dynamic array.\n");

  // Create tokens dynamic array
  CAM_TEST_ASSERT_SUCCESS(cam_type_create_dyn_arr(
      &context.tokens, sizeof(wil_lexer_token_t), WIL_LEXER_TOKEN_INI_LEN));
  CAM_TEST_ASSERT_PREV_ERR_NOT(CAM_ERR_MEM_ALLOC);

  printf("Created dynamic array.\n");

  // Add token
  CAM_TEST_ASSERT_SUCCESS(wil_lexer_add_token_literal(
      &context, WIL_LEXER_TOKEN_INTEGER, (wil_lexer_literal_t){.integer = 10}));
  CAM_TEST_ASSERT(cam_type_len_dyn_arr(&context.tokens) == 1);
  wil_lexer_token_t *token = cam_type_get_dyn_arr(&context.tokens, 0);
  wil_lexer_token_dump_int(token);
  CAM_TEST_ASSERT_NOT_NULL(token);
  CAM_TEST_ASSERT(token->type == WIL_LEXER_TOKEN_INTEGER);
  CAM_TEST_ASSERT(token->literal.integer == 10);
  CAM_TEST_ASSERT(token->lexeme.str == context.source);
  CAM_TEST_ASSERT(token->lexeme.len == 2);

  CAM_TEST_STOP_SUCCESS();
}
