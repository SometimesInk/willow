#include <camellia/camellia.h>
#include <stdarg.h>
#include <stdio.h>
#include <willow/ast/expressions.h>
#include <willow/ast/pretty_print.h>

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
