#include <camellia/camellia.h>
#include <camellia/type/arena.h>
#include <stdio.h>
#include <willow/ast/expressions.h>
#include <willow/ast/parser.h>
#include <willow/ast/parser_utils.h>
#include <willow/err/err.h>
#include <willow/lexer/lexer.h>

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
  node->unary.op = tok;
  node->unary.expr = right;
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
  printf("wil_ast_parse_expr@%lu\n", context->index);
  return wil_ast_parse_equality(context);
  printf("wil_ast_parse_expr@end\n");
}

wil_ast_expr_t *wil_ast_parse_equality(wil_ast_parser_context_t *context) {
  printf("wil_ast_parse_equality@%lu\n", context->index);
  wil_ast_expr_t *expr = wil_ast_parse_comparison(context);
  if (expr == CAM_NULL)
    return CAM_NULL;

  while (wil_ast_match(context, 2, WIL_LEXER_TOKEN_BANG_EQUAL,
                       WIL_LEXER_TOKEN_EQUAL_EQUAL)) {
    printf("|matched equality keyword.\n");
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

  printf("wil_ast_parse_equality@end\n");
  return expr;
}

wil_ast_expr_t *wil_ast_parse_comparison(wil_ast_parser_context_t *context) {
  printf("wil_ast_parse_comparison@%lu\n", context->index);
  wil_ast_expr_t *expr = wil_ast_parse_term(context);
  if (expr == CAM_NULL)
    return CAM_NULL;

  while (wil_ast_match(context, 4, WIL_LEXER_TOKEN_GREATER,
                       WIL_LEXER_TOKEN_GREATER_EQUAL, WIL_LEXER_TOKEN_LESS,
                       WIL_LEXER_TOKEN_LESS_EQUAL)) {
    printf("|matched comparison keyword.\n");
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

  printf("wil_ast_parse_comparison@end\n");
  return expr;
}

wil_ast_expr_t *wil_ast_parse_term(wil_ast_parser_context_t *context) {
  printf("wil_ast_parse_term@%lu\n", context->index);
  wil_ast_expr_t *expr = wil_ast_parse_factor(context);
  if (expr == CAM_NULL)
    return CAM_NULL;

  while (
      wil_ast_match(context, 2, WIL_LEXER_TOKEN_MINUS, WIL_LEXER_TOKEN_PLUS)) {
    printf("|matched term keyword.\n");
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

  printf("wil_ast_parse_term@end\n");
  wil_ast_pretty_print(expr);
  return expr;
}

wil_ast_expr_t *wil_ast_parse_factor(wil_ast_parser_context_t *context) {
  printf("wil_ast_parse_factor@%lu\n", context->index);
  wil_ast_expr_t *expr = wil_ast_parse_unary(context);
  if (expr == CAM_NULL)
    return CAM_NULL;

  while (
      wil_ast_match(context, 2, WIL_LEXER_TOKEN_SLASH, WIL_LEXER_TOKEN_STAR)) {
    printf("|matched factor keyword.\n");
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

  printf("wil_ast_parse_factor@end\n");
  return expr;
}

wil_ast_expr_t *wil_ast_parse_unary(wil_ast_parser_context_t *context) {
  printf("wil_ast_parse_unary@%lu\n", context->index);
  if (wil_ast_match(context, 2, WIL_LEXER_TOKEN_BANG, WIL_LEXER_TOKEN_MINUS)) {
    printf("|matched unary keyword.\n");
    wil_lexer_token_t *tok = wil_ast_previous(context);
    if (tok == CAM_NULL)
      return CAM_NULL;
    wil_ast_expr_t *right = wil_ast_parse_unary(context);
    if (right == CAM_NULL)
      return CAM_NULL;
    wil_ast_expr_t *node = wil_ast_mk_unary_(context, tok, right);
    return node;
  }

  printf("wil_ast_parse_unary@end\n");
  return wil_ast_parse_primary(context);
}

wil_ast_expr_t *wil_ast_parse_primary(wil_ast_parser_context_t *context) {
  printf("wil_ast_parse_primary@%lu\n", context->index);
  if (wil_ast_match(context, 6, WIL_LEXER_TOKEN_KW_FALSE,
                    WIL_LEXER_TOKEN_KW_TRUE, WIL_LEXER_TOKEN_KW_NULL,
                    WIL_LEXER_TOKEN_INTEGER, WIL_LEXER_TOKEN_STRING,
                    WIL_LEXER_TOKEN_DECIMAL)) {
    printf("|matched literal keyword.\n");
    wil_lexer_token_t *tok = wil_ast_previous(context);
    if (tok == CAM_NULL)
      return CAM_NULL;
    return wil_ast_mk_lit_(context, tok);
  }

  if (wil_ast_match(context, 1, WIL_LEXER_TOKEN_LEFT_PAREN)) {
    printf("|matched left_paren keyword.\n");
    wil_ast_expr_t *expr = wil_ast_parse_expr(context);
    printf("|matched left_paren keyword (end of expr).\n");
    wil_ast_pretty_print(expr);
    putc('\n', stdout);
    if (expr == CAM_NULL)
      return CAM_NULL;
    if (wil_ast_consume(context, WIL_LEXER_TOKEN_RIGHT_PAREN,
                        WIL_ERR_CODE_RIGHT_PARAN_EXPECTED) == CAM_NULL)
      return CAM_NULL;
    printf("|matched right_paren keyword <- good thing.\n");
    return wil_ast_mk_group_(context, expr);
  }

  printf("wil_ast_parse_primary@end <- ERROR\n");
  return CAM_NULL;
}

wil_ast_parser_context_t
wil_ast_create_parser_context(wil_err_context_t *err,
                              wil_lexer_context_t *context) {
  wil_ast_parser_context_t c = {
      .err = err, .index = 0, .tokens = context->tokens, .ar = CAM_NULL};
  cam_type_create_arena(&c.ar, WIL_AST_DEF_AR_SIZE);

  return c;
}

void wil_ast_dispose_parser_context(wil_ast_parser_context_t *context) {
  cam_type_free_arena(&context->ar);
}
