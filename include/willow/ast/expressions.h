#ifndef WILLOW__AST_EXPRESSION_TYPES_H__
#define WILLOW__AST_EXPRESSION_TYPES_H__

#include <camellia/type/int.h>
#include <willow/lexer/tokens.h>

struct wil_ast_expr_s;
#define X(name, components) struct wil_ast_##name##_s;
#include <willow/ast/expressions.inc>
#undef X

typedef cam_cptr_t wil_ast_visitor_out_t;

struct wil_ast_visitor_s;
typedef struct wil_ast_visitor_s {
#define X(name, components)                                                    \
  wil_ast_visitor_out_t (*visit_##name)(struct wil_ast_visitor_s * visitor,    \
                                        struct wil_ast_##name##_s *);
#include <willow/ast/expressions.inc>
#undef X
} wil_ast_visitor_t;

typedef struct wil_ast_expr_s {
  wil_ast_visitor_out_t (*accept)(struct wil_ast_expr_s *, wil_ast_visitor_t *);
} wil_ast_expr_t;

#define X(name, components)                                                    \
  typedef struct wil_ast_##name##_s {                                          \
    wil_ast_expr_t base;                                                       \
    components                                                                 \
  } wil_ast_##name##_t;
#include <willow/ast/expressions.inc>
#undef X

#define X(name, components)                                                    \
  extern wil_ast_visitor_out_t wil_ast_accept_##name(                          \
      wil_ast_expr_t *expr, wil_ast_visitor_t *visitor);
#include <willow/ast/expressions.inc>
#undef X

// I see in the sky
// An X-macro calling my name
// I shall use it then

#endif /* WILLOW__AST_EXPRESSION_TYPES_H__ */
