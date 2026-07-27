#ifndef WILLOW__AST_EXPRESSION_TYPES_H__
#define WILLOW__AST_EXPRESSION_TYPES_H__

#include <willow/lexer/tokens.h>
union wil_ast_expr_u;

typedef enum {
  WIL_AST_EXPR_TYPE_UNARY,
  WIL_AST_EXPR_TYPE_BINARY,
  WIL_AST_EXPR_TYPE_GROUP,
  WIL_AST_EXPR_TYPE_LIT
} wil_ast_expr_type_t;

typedef struct {
  wil_ast_expr_type_t type;
  union wil_ast_expr_u *expr;
} wil_ast_expr_t;

typedef struct {
  wil_lexer_token_t op;
  wil_ast_expr_t *right;
} wil_ast_unary_t;

typedef struct {
  wil_ast_expr_t *left;
  wil_lexer_token_type_t op;
  wil_ast_expr_t *right;
} wil_ast_binary_t;

typedef struct {
  wil_ast_expr_t *expr;
} wil_ast_group_t;

typedef struct {
  wil_lexer_literal_t lit;
} wil_ast_lit_t;

union wil_ast_expr_u {
  wil_ast_unary_t unary;
  wil_ast_binary_t binary;
  wil_ast_group_t group;
  wil_ast_lit_t lit;
};

#endif /* WILLOW__AST_EXPRESSION_TYPES_H__ */
