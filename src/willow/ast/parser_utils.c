#include <camellia/camellia.h>
#include <camellia/type/dynamic_array.h>
#include <stdarg.h>
#include <willow/ast/parser_utils.h>
#include <willow/lexer/tokens.h>

cam_int_t wil_ast_is_at_end(wil_ast_parser_context_t *context) {
  return wil_ast_peek(context)->type == WIL_LEXER_TOKEN_EOF;
}

wil_lexer_token_t *wil_ast_peek(wil_ast_parser_context_t *context) {
  return (wil_lexer_token_t *)cam_type_get_dyn_arr(&context->arr,
                                                   context->index);
}

wil_lexer_token_t *wil_ast_previous(wil_ast_parser_context_t *context) {
  return (wil_lexer_token_t *)cam_type_get_dyn_arr(&context->arr,
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

  for (cam_size_t i = 0; i < n_expr; ++i) {
    wil_lexer_token_type_t type = va_arg(argp, wil_lexer_token_type_t);
    if (wil_ast_check(context, type)) {
      // Advance
      return CAM_TRUE;
    }
  }

  va_end(argp);
  return CAM_FALSE;
}

CAM_STATIC void wil_ast_parenthesize_(cam_cptr_t name, cam_size_t n_expr, ...) {
  va_list argp;
  va_start(argp, n_expr);

  printf("(");

  if (name != CAM_NULL)
    printf("%s", name);

  for (cam_size_t i = 0; i < n_expr; ++i) {
    printf(" ");
    wil_ast_expr_t *expr = va_arg(argp, wil_ast_expr_t *);

    wil_ast_pretty_print(expr);
  }

  printf(")");

  va_end(argp);
}

void wil_ast_pretty_print(wil_ast_expr_t *expr) {
  switch (expr->type) {
  case WIL_AST_EXPR_TYPE_UNARY:
    wil_ast_parenthesize_(expr->base.unary->op.lexeme.str, 1,
                          expr->base.unary->expr);
    break;
  case WIL_AST_EXPR_TYPE_BINARY:
    wil_ast_parenthesize_(expr->base.binary->op.lexeme.str, 2,
                          expr->base.binary->left, expr->base.binary->right);
    break;
  case WIL_AST_EXPR_TYPE_GROUP:
    wil_ast_parenthesize_("group", 1, expr);
    break;
  case WIL_AST_EXPR_TYPE_LIT:
    printf("%s", expr->base.lit->token.lexeme.str);
    break;
  };
}
