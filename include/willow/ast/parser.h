#ifndef WIL__AST_PARSER_H__
#define WIL__AST_PARSER_H__

#include <camellia/camellia.h>
#include <camellia/type/dynamic_array.h>
#include <willow/ast/expressions.h>

typedef struct {
  cam_size_t index;
  cam_type_dyn_arr_t arr;
} wil_ast_parser_context_t;

extern cam_out_t wil_ast_parse(wil_ast_parser_context_t *context,
                               wil_ast_expr_t *expr_out);

extern wil_ast_expr_t *wil_ast_parse_expr(wil_ast_parser_context_t *context);

extern wil_ast_expr_t *
wil_ast_parse_equality(wil_ast_parser_context_t *context);

extern wil_ast_expr_t *
wil_ast_parse_comparison(wil_ast_parser_context_t *context);

extern wil_ast_expr_t *wil_ast_parse_term(wil_ast_parser_context_t *context);

extern wil_ast_expr_t *wil_ast_parse_factor(wil_ast_parser_context_t *context);

extern wil_ast_expr_t *wil_ast_parse_unary(wil_ast_parser_context_t *context);

extern wil_ast_expr_t *wil_ast_parse_pimary(wil_ast_parser_context_t *context);

#endif /* WIL__AST_PARSER_H__ */
