#ifndef WIL__AST_PARSER_UTILS_H__
#define WIL__AST_PARSER_UTILS_H__

#include <willow/ast/parser.h>
#include <willow/lexer/tokens.h>

extern cam_int_t wil_ast_is_at_end(wil_ast_parser_context_t *context);

extern wil_lexer_token_t *wil_ast_peek(wil_ast_parser_context_t *context);

extern wil_lexer_token_t *wil_ast_previous(wil_ast_parser_context_t *context);

extern wil_lexer_token_t *wil_ast_advance(wil_ast_parser_context_t *context);

extern cam_int_t wil_ast_check(wil_ast_parser_context_t *context,
                               wil_lexer_token_type_t type);

extern cam_int_t wil_ast_match(wil_ast_parser_context_t *context,
                               cam_size_t n_expr, ...);

extern void wil_ast_pretty_print(wil_ast_expr_t *expr);

#endif /* WIL__AST_PARSER_UTILS_H__ */
