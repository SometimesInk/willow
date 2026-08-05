#include <camellia/camellia.h>
#include <camellia/type/arena.h>
#include <stdlib.h>
#include <willow/ast/expressions.h>
#include <willow/ast/parser.h>
#include <willow/ast/parser_utils.h>
#include <willow/lexer/tokens.h>

/*
typedef struct {
  wil_ast_expr_type_t type;
  union wil_ast_expr_u base;
} wil_ast_expr_t;
*/

#define WIL_AST_AR_ALLOC(type_)                                                \
  cam_type_alloc_arena(&context->ar, sizeof(type_), _Alignof(type_))

CAM_STATIC wil_ast_expr_t *wil_ast_mk_bin_(wil_ast_parser_context_t *context,
                                           wil_ast_expr_t *left,
                                           wil_lexer_token_t *tok,
                                           wil_ast_expr_t *right) {
  wil_ast_expr_t *node = WIL_AST_AR_ALLOC(wil_ast_expr_t);
  if (node == CAM_NULL)
    return CAM_NULL;
  node->type = WIL_AST_EXPR_TYPE_BINARY;
  node->binary.left = left;
  node->binary.op = tok;
  node->binary.right = right;
  return node;
}

CAM_STATIC wil_ast_expr_t *wil_ast_mk_unary_(wil_ast_parser_context_t *context,
                                             wil_lexer_token_t *tok,
                                             wil_ast_expr_t *right) {
  wil_ast_expr_t *node = WIL_AST_AR_ALLOC(wil_ast_expr_t);
  if (node == CAM_NULL)
    return CAM_NULL;
  node->type = WIL_AST_EXPR_TYPE_UNARY;
  node->binary.op = tok;
  node->binary.right = right;
  return node;
}

CAM_STATIC wil_ast_expr_t *wil_ast_mk_lit_(wil_ast_parser_context_t *context,
                                           wil_lexer_token_t *tok) {
  wil_ast_expr_t *node = WIL_AST_AR_ALLOC(wil_ast_expr_t);
  if (node == CAM_NULL)
    return CAM_NULL;
  node->type = WIL_AST_EXPR_TYPE_LIT;
  node->lit.token = *tok;
  return node;
}

CAM_STATIC wil_ast_expr_t *wil_ast_mk_group_(wil_ast_parser_context_t *context,
                                             wil_ast_expr_t *expr) {
  wil_ast_expr_t *node = WIL_AST_AR_ALLOC(wil_ast_expr_t);
  if (node == CAM_NULL)
    return CAM_NULL;
  node->type = WIL_AST_EXPR_TYPE_GROUP;
  node->group.expr = expr;
  return node;
}

wil_ast_expr_t *wil_ast_parse_expr(wil_ast_parser_context_t *context) {
  return wil_ast_parse_equality(context);
}

wil_ast_expr_t *wil_ast_parse_equality(wil_ast_parser_context_t *context) {
  wil_ast_expr_t *expr = wil_ast_parse_comparison(context);
  if (expr == CAM_NULL)
    return CAM_NULL;

  while (wil_ast_match(context, 2, WIL_LEXER_TOKEN_BANG_EQUAL,
                       WIL_LEXER_TOKEN_EQUAL_EQUAL)) {
    wil_lexer_token_t *tok = wil_ast_previous(context);
    if (tok == CAM_NULL)
      return CAM_NULL;
    wil_ast_expr_t *right = wil_ast_parse_comparison(context);
    if (right == CAM_NULL)
      return CAM_NULL;
    wil_ast_expr_t *node = wil_ast_mk_bin_(context, expr, tok, right);
    if (node == CAM_NULL)
      return CAM_NULL;
    expr = node;
  }

  return expr;
}

wil_ast_expr_t *wil_ast_parse_comparison(wil_ast_parser_context_t *context) {
  wil_ast_expr_t *expr = wil_ast_parse_term(context);
  if (expr == CAM_NULL)
    return CAM_NULL;

  while (wil_ast_match(context, 4, WIL_LEXER_TOKEN_GREATER,
                       WIL_LEXER_TOKEN_GREATER_EQUAL, WIL_LEXER_TOKEN_LESS,
                       WIL_LEXER_TOKEN_LESS_EQUAL)) {
    wil_lexer_token_t *tok = wil_ast_previous(context);
    if (tok == CAM_NULL)
      return CAM_NULL;
    wil_ast_expr_t *right = wil_ast_parse_term(context);
    if (right == CAM_NULL)
      return CAM_NULL;
    wil_ast_expr_t *node = wil_ast_mk_bin_(context, expr, tok, right);
    if (node == CAM_NULL)
      return CAM_NULL;
    expr = node;
  }

  return expr;
}

wil_ast_expr_t *wil_ast_parse_term(wil_ast_parser_context_t *context) {
  wil_ast_expr_t *expr = wil_ast_parse_factor(context);
  if (expr == CAM_NULL)
    return CAM_NULL;

  while (
      wil_ast_match(context, 2, WIL_LEXER_TOKEN_MINUS, WIL_LEXER_TOKEN_PLUS)) {
    wil_lexer_token_t *tok = wil_ast_previous(context);
    if (tok == CAM_NULL)
      return CAM_NULL;
    wil_ast_expr_t *right = wil_ast_parse_factor(context);
    if (right == CAM_NULL)
      return CAM_NULL;
    wil_ast_expr_t *node = wil_ast_mk_bin_(context, expr, tok, right);
    if (node == CAM_NULL)
      return CAM_NULL;
    expr = node;
  }

  return expr;
}

wil_ast_expr_t *wil_ast_parse_factor(wil_ast_parser_context_t *context) {
  wil_ast_expr_t *expr = wil_ast_parse_unary(context);
  if (expr == CAM_NULL)
    return CAM_NULL;

  while (
      wil_ast_match(context, 2, WIL_LEXER_TOKEN_SLASH, WIL_LEXER_TOKEN_STAR)) {
    wil_lexer_token_t *tok = wil_ast_previous(context);
    if (tok == CAM_NULL)
      return CAM_NULL;
    wil_ast_expr_t *right = wil_ast_parse_unary(context);
    if (right == CAM_NULL)
      return CAM_NULL;
    wil_ast_expr_t *node = wil_ast_mk_bin_(context, expr, tok, right);
    if (node == CAM_NULL)
      return CAM_NULL;
    expr = node;
  }

  return expr;
}

wil_ast_expr_t *wil_ast_parse_unary(wil_ast_parser_context_t *context) {
  if (wil_ast_match(context, 2, WIL_LEXER_TOKEN_BANG, WIL_LEXER_TOKEN_MINUS)) {
    wil_lexer_token_t *tok = wil_ast_previous(context);
    if (tok == CAM_NULL)
      return CAM_NULL;
    wil_ast_expr_t *right = wil_ast_parse_unary(context);
    if (right == CAM_NULL)
      return CAM_NULL;
    wil_ast_expr_t *node = wil_ast_mk_unary_(context, tok, right);
    return node;
  }

  return wil_ast_parse_primary(context);
}

wil_ast_expr_t *wil_ast_parse_pimary(wil_ast_parser_context_t *context) {
  if (wil_ast_match(context, 6, WIL_LEXER_TOKEN_KW_FALSE,
                    WIL_LEXER_TOKEN_KW_TRUE, WIL_LEXER_TOKEN_KW_NULL,
                    WIL_LEXER_TOKEN_INTEGER, WIL_LEXER_TOKEN_STRING,
                    WIL_LEXER_TOKEN_FLOAT)) {
    wil_lexer_token_t *tok = wil_ast_previous(context);
    if (tok == CAM_NULL)
      return CAM_NULL;
    return wil_ast_mk_lit_(context, tok);
  }

  if (wil_ast_match(context, 1, WIL_LEXER_TOKEN_LEFT_PAREN)) {
    wil_ast_expr_t *expr = wil_ast_parse_expr(context);
    if (expr == CAM_NULL)
      return CAM_NULL;
    if (wil_ast_consume(context, WIL_LEXER_TOKEN_RIGHT_PAREN,
                        WIL_AST_ERR_RIGHT_PARAN_EXPECTED) == CAM_NULL)
      return CAM_NULL;
    return wil_ast_mk_group_(context, expr);
  }

  return CAM_NULL;
}
