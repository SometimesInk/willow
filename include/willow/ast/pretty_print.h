#ifndef WILLOW__AST_PRETTY_PRINT_H__
#define WILLOW__AST_PRETTY_PRINT_H__

#include <willow/ast/expressions.h>

extern void wil_ast_parenthesize(wil_ast_visitor_t *visitor, cam_cptr_t name,
                                 cam_size_t n_expr, ...);

extern void wil_ast_pretty_print(wil_ast_expr_t expr);

#define X(name, components)                                                    \
  extern wil_ast_visitor_out_t wil_ast_pretty_print_visit_##name(              \
      wil_ast_visitor_t *visitor, wil_ast_##name##_t *expr);
#include <willow/ast/expressions.inc>
#undef X

#endif /* WILLOW__AST_PRETTY_PRINT_H__ */
