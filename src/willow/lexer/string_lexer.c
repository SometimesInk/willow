#include "willow/lexer/tokens.h"
#include <camellia/camellia.h>
#include <camellia/type/string.h>
#include <willow/lexer/lexer_utils.h>
#include <willow/lexer/string_lexer.h>

void wil_lexer_string_literal(wil_lexer_context_t *context) {
  while (wil_lexer_peek(context) != '"' && !wil_lexer_is_at_end(context)) {
    if (wil_lexer_peek(context) == '\n')
      context->line++;
    wil_lexer_advance(context);
  }

  if (wil_lexer_is_at_end(context)) {
    wil_lexer_err(context, "Unterminated string.");
    return;
  }

  // Closing quotation
  wil_lexer_advance(context);

  // Trim surrounding quotes for literal token
  cam_str_t literal;
  cam_type_substring(&literal, context->start + 1, 0,
                     (cam_size_t)(context->current - context->start));
  wil_lexer_add_token_literal(context, WIL_LEXER_TOKEN_STRING,
                              (wil_lexer_literal_t){.string = literal});
}
