#define CAM_TEST

#include <camellia/camellia.h>
#include <camellia/err/err.h>
#include <camellia/test/test.h>
#include <camellia/type/dynamic_array.h>
#include <camellia/type/string.h>
#include <stdio.h>
#include <willow/ast/expressions.h>
#include <willow/ast/parser.h>
#include <willow/ast/parser_utils.h>
#include <willow/io/io.h>
#include <willow/lexer/lexer.h>
#include <willow/lexer/lexer_utils.h>
#include <willow/lexer/tokens.h>

wil_lexer_context_t c_lexer;
wil_ast_parser_context_t c_parser;

#undef CAM_TEST_RETURN
#define CAM_TEST_RETURN(v)                                                     \
  do {                                                                         \
    wil_lexer_dispose_context(&c_lexer);                                       \
    wil_ast_dispose_parser_context(&c_parser);                                 \
    return v;                                                                  \
  } while (0)

cam_test_result_t main(void) {
  // Create context
  cam_cptr_t source = "1.23+4+\"helo\"";
  c_lexer = wil_lexer_create_context(source);
  CAM_TEST_ASSERT_NOT_NULL(c_lexer.source);
  CAM_TEST_ASSERT_NOT_NULL(c_lexer.current);
  CAM_TEST_ASSERT_NOT_NULL(c_lexer.start);

  // Create tokens dynamic array
  CAM_TEST_ASSERT_SUCCESS(cam_type_create_dyn_arr(
      &c_lexer.tokens, sizeof(wil_lexer_token_t), WIL_LEXER_TOKEN_INI_LEN));
  CAM_TEST_ASSERT_PREV_ERR_NOT(CAM_ERR_MEM_ALLOC);

  wil_lexer_scan_tokens(&c_lexer, &c_lexer.tokens);

  for (int i = 0; i < cam_type_len_dyn_arr(&c_lexer.tokens); i++) {
    wil_lexer_token_t *token =
        (wil_lexer_token_t *)cam_type_get_dyn_arr(&c_lexer.tokens, i);
    printf("At i=%d: {type=%d}\n", i, token->type);
  }

  c_parser = wil_ast_create_parser_context(&c_lexer);

  wil_ast_expr_t *expr = wil_ast_parse_expr(&c_parser);

  wil_ast_pretty_print(expr);

  CAM_TEST_STOP_SUCCESS();
}
