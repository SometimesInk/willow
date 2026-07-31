#define CAM_TEST

#include <camellia/test/test.h>
#include <willow/ast/expressions.h>
#include <willow/ast/pretty_print.h>
#include <willow/lexer/tokens.h>

cam_test_result_t main(void) {
  wil_ast_visitor_t ast_print_visitor = {
      wil_ast_pretty_print_visit_unary, wil_ast_pretty_print_visit_binary,
      wil_ast_pretty_print_visit_group, wil_ast_pretty_print_visit_lit};

  wil_lexer_token_t lit1_token = {
      WIL_LEXER_TOKEN_INTEGER, {4, "172"}, 0, {.integer = 172}};
  wil_lexer_token_t lit2_token = {
      WIL_LEXER_TOKEN_STRING, {7, "Willow"}, 0, {.string = {7, "Willow"}}};

  wil_lexer_token_t bin_token = {
      WIL_LEXER_TOKEN_PLUS, {2, "+"}, 0, {.integer = 1}};

  wil_ast_lit_t lit1 = {{wil_ast_accept_lit}, lit1_token};
  wil_ast_lit_t lit2 = {{wil_ast_accept_lit}, lit2_token};

  wil_ast_binary_t bin = {
      {wil_ast_accept_binary}, bin_token, &lit1.base, &lit2.base};

  bin.base.accept(&bin.base, &ast_print_visitor);

  CAM_TEST_STOP_SUCCESS();
}
