#ifndef WILLOW__LEXER_TOKENS_H__
#define WILLOW__LEXER_TOKENS_H__

#include <camellia/camellia.h>
#include <camellia/type/dynamic_array.h>
#include <willow/lexer/lexer.h>

typedef enum {
  WIL_LEXER_TOKEN_NONE,
#define X(token) token,
#include <willow/lexer/token_definitions.inc>
#undef X
#define X(token, string) token,
#include <willow/lexer/keyword_definitions.inc>
#undef X
  /**
   * @brief Length of `wil_lexer_token_t` enum.
   */
  WIL_LEXER_TOKEN_LEN,
} wil_lexer_token_type_t;

typedef union {
  cam_str_t string;
  cam_int_t integer;
  float decimal;
} wil_lexer_literal_t;

#define WIL_LEXER_TOKEN_INI_LEN 8

typedef struct {
  wil_lexer_token_type_t type;
  cam_str_t lexeme;
  cam_int_t line;
  /**
   * @warn If `type == WIL_LEXER_TOKEN_STRING`, then
   * `literal.string.str` is owned by this token and will be released
   * by `wil_lexer_dispose_context()` using `free()`.
   *
   * Therefore, `literal.string.str` must either be NULL or point to
   * memory that may legally be passed to `free()`. Borrowed pointers,
   * string literals, and pointers into the source buffer are invalid.
   */
  wil_lexer_literal_t literal;
  ;
} wil_lexer_token_t;

extern cam_out_t wil_lexer_scan_one_token(wil_lexer_context_t *context,
                                          cam_type_dyn_arr_t *arr);

extern void wil_lexer_token_dump_none(const wil_lexer_token_t *token);

extern void wil_lexer_token_dump_str(const wil_lexer_token_t *token);

extern void wil_lexer_token_dump_int(const wil_lexer_token_t *token);

extern void wil_lexer_token_dump_dec(const wil_lexer_token_t *token);

#endif /* WILLOW__LEXER_TOKENS_H__ */
