#include <camellia/camellia.h>
#include <camellia/type/dynamic_array.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <willow/ast/parser_utils.h>
#include <willow/err/err.h>
#include <willow/lexer/tokens.h>

wil_lexer_token_t *wil_ast_consume(wil_ast_parser_context_t *c,
                                   wil_lexer_token_type_t type,
                                   wil_err_code_t err) {
  wil_lexer_token_t *token = wil_ast_peek(c);
  if (token->type == type) {
    return wil_ast_advance(c);
  }

  wil_err_emit_t(c->err, err, token);
  wil_lexer_token_dump(token);
  return CAM_NULL;
}

cam_int_t wil_ast_is_at_end(wil_ast_parser_context_t *context) {
  return wil_ast_peek(context)->type == WIL_LEXER_TOKEN_EOF;
}

wil_lexer_token_t *wil_ast_peek(wil_ast_parser_context_t *context) {
  return (wil_lexer_token_t *)cam_type_get_dyn_arr(&context->tokens,
                                                   context->index);
}

wil_lexer_token_t *wil_ast_previous(wil_ast_parser_context_t *context) {
  return (wil_lexer_token_t *)cam_type_get_dyn_arr(&context->tokens,
                                                   context->index - 1);
}

wil_lexer_token_t *wil_ast_advance(wil_ast_parser_context_t *context) {
  if (!wil_ast_is_at_end(context))
    context->index++;
  return wil_ast_previous(context);
}

cam_int_t wil_ast_check(wil_ast_parser_context_t *context,
                        wil_lexer_token_type_t type) {
  if (wil_ast_is_at_end(context))
    return CAM_FALSE;
  return wil_ast_peek(context)->type == type;
}

cam_int_t wil_ast_match(wil_ast_parser_context_t *context, cam_size_t n_expr,
                        ...) {
  va_list argp;
  va_start(argp, n_expr);

  cam_int_t matched = CAM_FALSE;

  for (cam_size_t i = 0; i < n_expr; ++i) {
    wil_lexer_token_type_t type = (wil_lexer_token_type_t)va_arg(argp, int);
    if (!wil_ast_check(context, type))
      continue;

    wil_ast_advance(context);
    matched = CAM_TRUE;
    break;
  }

  va_end(argp);
  return matched;
}

CAM_STATIC void wil_ast_pretty_print_expr_(wil_ast_expr_t *expr);

CAM_STATIC void wil_ast_parenthesize_(cam_str_t name, cam_size_t n_expr, ...) {
  va_list argp;
  va_start(argp, n_expr);

  printf("(");

  if (name.str != CAM_NULL)
    printf("%.*s", (cam_int_t)name.len, name.str);

  for (cam_size_t i = 0; i < n_expr; ++i) {
    if (!(i == 0 && name.str == CAM_NULL))
      printf(" ");
    wil_ast_expr_t *expr = va_arg(argp, wil_ast_expr_t *);

    wil_ast_pretty_print_expr_(expr);
  }

  printf(")");

  va_end(argp);
}

#define WIL_AST_PARENTHESIZE_PTR_(name, n_expr, ...)                           \
  do {                                                                         \
    cam_str_t str = {.str = name, .len = strlen(name)};                        \
    wil_ast_parenthesize_(str, n_expr, __VA_ARGS__);                           \
  } while (0)

CAM_STATIC void wil_ast_pretty_print_expr_(wil_ast_expr_t *expr) {
  switch (expr->type) {
  case WIL_AST_EXPR_TYPE_UNARY:
    wil_ast_parenthesize_(expr->unary.op->lexeme, 1, expr->unary.expr);
    break;
  case WIL_AST_EXPR_TYPE_BINARY:
    wil_ast_parenthesize_(expr->binary.op->lexeme, 2, expr->binary.left,
                          expr->binary.right);
    break;
  case WIL_AST_EXPR_TYPE_GROUP:
    WIL_AST_PARENTHESIZE_PTR_("group", 1, expr->group.expr);
    break;
  case WIL_AST_EXPR_TYPE_LIT:
    printf("%.*s", (cam_int_t)expr->lit.token.lexeme.len,
           expr->lit.token.lexeme.str);
    break;
  };
}

void wil_ast_pretty_print(wil_ast_expr_t *expr) {
  wil_ast_pretty_print_expr_(expr);
  putc('\n', stdout);
}
