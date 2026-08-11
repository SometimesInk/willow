#ifndef WILLOW__LEXER_LEXER_H__
#define WILLOW__LEXER_LEXER_H__

#include <camellia/camellia.h>
#include <camellia/type/dynamic_array.h>
#include <willow/err/err.h>
#include <willow/lexer/source.h>

typedef struct {
  wil_err_context_t *err;
  wil_lexer_src_t src;

  cam_type_dyn_arr_t tokens;

  /**
   * @brief Start of next token.
   */
  wil_lexer_sptr_t start;
  /**
   * @brief Current cursor position.
   */
  wil_lexer_sptr_t current;

  cam_int_t line;
} wil_lexer_context_t;

#define WIL_LEXER_TOKEN_ARR_INI_LEN 8

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

static const char *const wil_lexer_token_type_name[WIL_LEXER_TOKEN_LEN] = {
#define X(token) [token] = #token,
#include <willow/lexer/token_definitions.inc>
#undef X
#define X(token, string) [token] = #token,
#include <willow/lexer/keyword_definitions.inc>
#undef X
};

typedef union {
  cam_str_t s;
  cam_int_t i;
  double d;
} wil_lexer_literal_t;

typedef struct wil_lexer_token_s {
  wil_lexer_token_type_t type;
  wil_lexer_sspan_t lexeme;
  cam_int_t line;
  /**
   * @warn If `type == WIL_LEXER_TOKEN_STRING`, then
   * `literal.string.str` is owned by this token and will be released
   * by `wil_lexer_dispose_context()` using `free()`. Therefore,
   * `literal.string.str` must either be NULL or point to memory that may
   * legally be passed to `free()`. It thus can not be a source pointer.
   */
  wil_lexer_literal_t lit;
} wil_lexer_token_t;

extern wil_lexer_context_t wil_lexer_create_context(wil_err_context_t *,
                                                    const cam_cptr_t src);

extern void wil_lexer_dispose_context(wil_lexer_context_t *);

extern cam_out_t wil_lexer_scan_tokens(wil_lexer_context_t *);

extern cam_out_t wil_lexer_scan_one_token(wil_lexer_context_t *);

#endif /* WILLOW__LEXER_LEXER_H__ */
