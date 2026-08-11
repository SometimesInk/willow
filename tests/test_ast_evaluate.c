#define CAM_TEST

#include <camellia/camellia.h>
#include <camellia/err/err.h>
#include <camellia/test/test.h>
#include <camellia/type/dynamic_array.h>
#include <camellia/type/string.h>
#include <willow/ast/evaluate.h>
#include <willow/ast/expressions.h>
#include <willow/ast/parser.h>
#include <willow/ast/parser_utils.h>
#include <willow/err/err.h>
#include <willow/io/io.h>
#include <willow/lexer/lexer.h>
#include <willow/lexer/lexer_utils.h>
#include <willow/lexer/tokens.h>

char *source = "-13 == (1 + 1 + 1 + 1 - 4 * 4) - 1";

wil_err_context_t c_err;
wil_lexer_context_t c_lexer;
wil_ast_parser_context_t c_parser;
wil_ast_interpreter_context_t c_interpreter;

#undef CAM_TEST_RETURN
#define CAM_TEST_RETURN(v)                                                     \
  do {                                                                         \
    wil_lexer_dispose_context(&c_lexer);                                       \
    wil_ast_dispose_parser_context(&c_parser);                                 \
    wil_err_dispose_context(&c_err);                                           \
    return v;                                                                  \
  } while (0)

cam_test_result_t main(void) {
  c_err = wil_err_create_context();

  c_lexer = wil_lexer_create_context(source);
  CAM_TEST_ASSERT_SUCCESS(cam_type_create_dyn_arr(
      &c_lexer.tokens, sizeof(wil_lexer_token_t), WIL_LEXER_TOKEN_INI_LEN));
  CAM_TEST_ASSERT_PREV_ERR_NOT(CAM_ERR_MEM_ALLOC);

  wil_lexer_scan_tokens(&c_lexer, &c_lexer.tokens);

  c_parser = wil_ast_create_parser_context(&c_err, &c_lexer);

  wil_ast_expr_t *expr = wil_ast_parse_expr(&c_parser);
  wil_ast_pretty_print(expr);

  // Evaluate
  wil_ast_print_eval_value(wil_ast_eval_expr(&c_interpreter, expr));

  CAM_TEST_STOP_SUCCESS();
}
