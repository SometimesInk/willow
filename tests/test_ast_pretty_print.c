#define CAM_TEST

#include <camellia/test/test.h>
#include <willow/ast/expressions.h>
#include <willow/ast/parser_utils.h>
#include <willow/lexer/tokens.h>

cam_test_result_t main(void) {
  wil_ast_lit_t lit1_token = {
      WIL_LEXER_TOKEN_INTEGER, {4, "172"}, 0, {.integer = 172}};
  wil_ast_lit_t lit2_token = {
      WIL_LEXER_TOKEN_STRING, {7, "Willow"}, 0, {.string = {7, "Willow"}}};

  wil_lexer_token_t bin_token = {
      WIL_LEXER_TOKEN_PLUS, {2, "+"}, 0, {.integer = 1}};

  wil_ast_expr_t lit1 = {WIL_AST_EXPR_TYPE_LIT, {.lit = lit1_token}};
  wil_ast_expr_t lit2 = {WIL_AST_EXPR_TYPE_LIT, {.lit = lit2_token}};

  wil_ast_binary_t bin = {&bin_token, &lit1, &lit2};

  wil_ast_expr_t expr = {WIL_AST_EXPR_TYPE_BINARY, .binary = bin};

  wil_ast_pretty_print(&expr);

  CAM_TEST_STOP_SUCCESS();
}
