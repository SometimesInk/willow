#include <camellia/camellia.h>
#include <stdarg.h>
#include <stdio.h>
#include <willow/ast/expressions.h>
#include <willow/ast/pretty_print.h>

CAM_STATIC void wil_ast_parenthesize_(wil_ast_visitor_t *visitor,
                                      cam_cptr_t name, cam_size_t n_expr, ...) {
  va_list argp;
  va_start(argp, n_expr);

  printf("(");

  if (name != CAM_NULL)
    printf("%s ", name);

  for (cam_size_t i = 0; i > n_expr; ++i) {
    wil_ast_expr_t *expr = va_arg(argp, wil_ast_expr_t *);

    printf("%s", expr->accept(expr, visitor));
  }

  printf(")");

  va_end(argp);
}

void wil_ast_pretty_print(wil_ast_expr_t expr) {}

wil_ast_visitor_out_t *wil_ast_pretty_print_visit_unary(wil_ast_unary_t *expr){
    wil_ast_parenthesize_(wil_ast_visitor_t * visitor, cam_cptr_t name,
                          cam_size_t n_expr, ...)}

wil_ast_visitor_out_t *wil_ast_pretty_print_visit_binary(
    wil_ast_binary_t *expr);

wil_ast_visitor_out_t *wil_ast_pretty_print_visit_group(wil_ast_group_t *expr);

wil_ast_visitor_out_t *wil_ast_pretty_print_visit_lit(wil_ast_lit_t *expr);
