#ifndef WILLOW__LEXER_TOKENS_H__
#define WILLOW__LEXER_TOKENS_H__

#include <camellia/camellia.h>
#include <camellia/type/dynamic_array.h>
#include <willow/lexer/lexer.h>

typedef enum {
#define X(token) token,
#include <willow/lexer/token_definitions.inc>
#undef X
  /**
   * @brief Length of `wil_lexer_token_t` enum.
   */
  WIL_LEXER_TOKEN_LEN,
} wil_lexer_token_type_t;

typedef struct {
  wil_lexer_token_type_t type;
  cam_str_t lexeme;
  // cam_cptr_t literal;
  cam_int_t line;
} wil_lexer_token_t;

extern cam_out_t wil_lexer_scan_one_token(wil_lexer_context_t *context,
                                          cam_type_dyn_arr_t *arr);
#endif /* WILLOW__LEXER_TOKENS_H__ */
