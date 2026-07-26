#include <camellia/camellia.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <willow/lexer/lexer.h>
#include <willow/lexer/lexer_utils.h>
#include <willow/lexer/literal_integer_and_float_lexer.h>
#include <willow/lexer/tokens.h>

void wil_lexer_parse_float(wil_lexer_context_t *context) {
  // Consume digits past decimal
  while (wil_lexer_is_digit(wil_lexer_peek(context))) {
    if (wil_lexer_is_at_end(context))
      break;
    wil_lexer_advance(context);
  }

  // Parse as float
  errno = 0;
  float val = strtof(context->start, CAM_NULL);
  if (errno != 0) {
    wil_lexer_err(context, "Failed to parse float.");
  }

  wil_lexer_add_token_literal(context, WIL_LEXER_TOKEN_FLOAT,
                              (wil_lexer_literal_t){.decimal = val});
  printf("float%f\n", val);
}

void wil_lexer_integer_and_float_literal(wil_lexer_context_t *context) {
  // Consume all digits
  while (wil_lexer_is_digit(wil_lexer_peek(context))) {
    if (wil_lexer_is_at_end(context))
      break;
    wil_lexer_advance(context);
  }

  // Parse floats
  if (wil_lexer_peek(context) == '.') {
    wil_lexer_advance(context);

    wil_lexer_parse_float(context);
    return;
  }

  // Parse as integer
  errno = 0;
  int val = strtol(context->start, CAM_NULL, 10);
  if (errno != 0)
    wil_lexer_err(context, "Failed to parse integer.");

  wil_lexer_add_token_literal(context, WIL_LEXER_TOKEN_INTEGER,
                              (wil_lexer_literal_t){.integer = val});
}
