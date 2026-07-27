#include "willow/lexer/tokens.h"
#include <camellia/camellia.h>
#include <camellia/type/string.h>
#include <stdlib.h>
#include <willow/lexer/keyword.h>

wil_lexer_token_type_t wil_lexer_keyword_check(cam_str_t lit, cam_cptr_t check,
                                               wil_lexer_token_type_t type) {
  return (cam_type_str_match(lit.str, check, strlen(check)))
             ? type
             : WIL_LEXER_TOKEN_IDENTIFIER;
}

wil_lexer_token_type_t
wil_lexer_identifier_is_keyword(wil_lexer_context_t *context) {
  wil_lexer_token_type_t out = WIL_LEXER_TOKEN_IDENTIFIER;

  // Get text literal
  cam_str_t lit = {};
  // TODO: Voiding substring will definitely cause trouble later
  // TODO: Check at least MEMALLOC
  // TODO: Add error signature to cam_type_substring docs
  (void)cam_type_substring(&lit, context->start, 0,
                           context->current - context->start);

#define X(token_type, token_string)                                            \
  out = wil_lexer_keyword_check(lit, token_string, token_type);
#include <willow/lexer/keyword_definitions.inc>
#undef X

  free(lit.str);
  return out;
}
