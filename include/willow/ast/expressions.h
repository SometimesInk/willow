#ifndef WILLOW__AST_EXPRESSION_TYPES_H__
#define WILLOW__AST_EXPRESSION_TYPES_H__

#include <camellia/type/int.h>
#include <willow/lexer/lexer.h>

typedef enum {
#define X(name, upper, components) WIL_AST_EXPR_TYPE_##upper,
#include <willow/ast/expressions.inc>
#undef X
} wil_ast_expr_type_t;

// Forward declare expr types for expr union

#define X(name, upper, components) struct wil_ast_##name##_s;
#include <willow/ast/expressions.inc>
#undef X
typedef struct wil_ast_expr_s wil_ast_expr_t;

// Definition for forward declared expr types

#define X(name, upper, components)                                             \
  typedef struct wil_ast_##name##_s {                                          \
    components                                                                 \
  } wil_ast_##name##_t;
#include <willow/ast/expressions.inc>
#undef X

// Expression declaration

typedef struct wil_ast_expr_s {
  wil_ast_expr_type_t type;
  union {
#define X(name, upper, components) struct wil_ast_##name##_s name;
#include <willow/ast/expressions.inc>
#undef X
  };
} wil_ast_expr_t;

// Here is my excuse for all these X-macros: laziness is the virtue of a good
// programmer

#endif /* WILLOW__AST_EXPRESSION_TYPES_H__ */
