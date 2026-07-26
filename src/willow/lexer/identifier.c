#include "willow/lexer/tokens.h"
#include <willow/lexer/identifier.h>
#include <willow/lexer/lexer_utils.h>

void wil_lexer_identifier(wil_lexer_context_t *context) {
  // Consume all alphanumeric characters in the identifier (which may be a
  // keyword at this point)
  while (wil_lexer_is_alphanumeric(wil_lexer_peek(context)))
    wil_lexer_advance(context);

  wil_lexer_add_token(context, WIL_LEXER_TOKEN_IDENTIFIER);
}
