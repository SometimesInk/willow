#include <camellia/camellia.h>
#include <camellia/type/string.h>
#include <stdio.h>
#include <willow/lexer/lexer_utils.h>
#include <willow/lexer/string_lexer.h>
#include <willow/lexer/tokens.h>

void wil_lexer_string_literal(wil_lexer_context_t *context) {
  while (context->current[0] != '"' && !wil_lexer_is_at_end(context)) {
    if (wil_lexer_peek(context) == '\n')
      context->line++;
    wil_lexer_advance(context);
  }

  if (wil_lexer_is_at_end(context)) {
    wil_lexer_err(context, "Unterminated string.");
    return;
  }

  wil_lexer_advance(context);

  cam_str_t literal;

  // -2 for both quotations, -1 to compensate for the previous advance
  cam_size_t end = (cam_size_t)(context->current - context->start - 2 - 1);
  cam_type_substring(&literal, context->start + 1, 0, end);

  wil_lexer_add_token_literal(context, WIL_LEXER_TOKEN_STRING,
                              (wil_lexer_literal_t){.string = literal});
}
