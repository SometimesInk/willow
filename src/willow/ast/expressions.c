#include <willow/ast/expressions.h>

#define X(name, components)                                                    \
  wil_ast_visitor_out_t wil_ast_accept_##name(wil_ast_expr_t *expr,            \
                                              wil_ast_visitor_t *visitor) {    \
    return visitor->visit_##name(visitor, (wil_ast_##name##_t *)expr);         \
  }
#include <willow/ast/expressions.inc>
#undef X
