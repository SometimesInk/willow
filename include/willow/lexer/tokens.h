#ifndef WILLOW__LEXER_TOKENS_H__
#define WILLOW__LEXER_TOKENS_H__

#include <camellia/camellia.h>
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
  cam_cptr_t lexeme;
  cam_cptr_t literal;
  cam_int_t line;
} wil_lexer_token_t;

// TODO: Continue from `Regular Language and Expressions 4.3`
// https://craftinginterpreters.com/scanning.html#regular-languages-and-expressions

#endif /* WILLOW__LEXER_TOKENS_H__ */
