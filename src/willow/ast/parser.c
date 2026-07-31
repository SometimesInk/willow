#include "willow/ast/expressions.h"
#include "willow/ast/parser_utils.h"
#include "willow/lexer/tokens.h"
#include <willow/ast/parser.h>

cam_out_t wil_ast_parse(wil_ast_parser_context_t *context,
                        wil_ast_expr_t *expr_out);

wil_ast_expr_t *wil_ast_parse_expr(wil_ast_parser_context_t *context) {
  return wil_ast_parse_equality(context);
}

wil_ast_expr_t *wil_ast_parse_equality(wil_ast_parser_context_t *context) {
  wil_ast_expr_t *expr = wil_ast_parse_comparison(context);

  while (wil_ast_match(context, 2, WIL_LEXER_TOKEN_BANG_EQUAL,
                       WIL_LEXER_TOKEN_EQUAL_EQUAL)) {
    wil_lexer_token_t *operator = wil_ast_previous(context);
    wil_ast_expr_t *right = wil_ast_parse_comparison(context);
    wil_ast_binary_t bin = {wil_ast_accept_binary, *operator, right, expr};
    expr = &bin.base;
  }

  return expr;
}

extern wil_ast_expr_t *
wil_ast_parse_comparison(wil_ast_parser_context_t *context) {
  wil_ast_expr_t *expr = wil_ast_parse_term(context);

  while (wil_ast_match(context, 4, WIL_LEXER_TOKEN_GREATER,
                       WIL_LEXER_TOKEN_GREATER_EQUAL, WIL_LEXER_TOKEN_LESS,
                       WIL_LEXER_TOKEN_LESS_EQUAL)) {
    wil_lexer_token_t *operator = wil_ast_previous(context);
    wil_ast_expr_t *right = wil_ast_parse_term(context);
    wil_ast_binary_t bin = {wil_ast_accept_binary, *operator, right, expr};
    expr = &bin.base;
  }

  return expr;
}

extern wil_ast_expr_t *wil_ast_parse_term(wil_ast_parser_context_t *context) {
  wil_ast_expr_t *expr = wil_ast_parse_factor(context);

  while (
      wil_ast_match(context, 2, WIL_LEXER_TOKEN_PLUS, WIL_LEXER_TOKEN_MINUS)) {
    wil_lexer_token_t *operator = wil_ast_previous(context);
    wil_ast_expr_t *right = wil_ast_parse_factor(context);
    wil_ast_binary_t bin = {wil_ast_accept_binary, *operator, right, expr};
    expr = &bin.base;
  }

  return expr;
}

extern wil_ast_expr_t *wil_ast_parse_factor(wil_ast_parser_context_t *context) {
  wil_ast_expr_t *expr = wil_ast_parse_unary(context);

  while (
      wil_ast_match(context, 2, WIL_LEXER_TOKEN_SLASH, WIL_LEXER_TOKEN_STAR)) {
    wil_lexer_token_t *operator = wil_ast_previous(context);
    wil_ast_expr_t *right = wil_ast_parse_unary(context);
    wil_ast_binary_t bin = {wil_ast_accept_binary, *operator, right, expr};
    expr = &bin.base;
  }

  return expr;
}

extern wil_ast_expr_t *wil_ast_parse_unary(wil_ast_parser_context_t *context) {
  while (
      wil_ast_match(context, 2, WIL_LEXER_TOKEN_BANG, WIL_LEXER_TOKEN_MINUS)) {
    wil_lexer_token_t *operator = wil_ast_previous(context);
    wil_ast_expr_t *right = wil_ast_parse_unary(context);
    wil_ast_binary_t bin = {wil_ast_accept_binary, *operator, right};
    return &bin.base;
  }

  return wil_ast_parse_primary(context);
}

extern wil_ast_expr_t *wil_ast_parse_pimary(wil_ast_parser_context_t *context) {
}
