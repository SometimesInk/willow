#include <camellia/camellia.h>
#include <camellia/err/err.h>
#include <camellia/type/dynamic_array.h>
#include <camellia/type/string.h>
#include <stdio.h>
#include <string.h>
#include <willow/lexer/lexer.h>
#include <willow/lexer/lexer_utils.h>
#include <willow/lexer/tokens.h>

void wil_lexer_err(wil_lexer_context_t *context, const cam_cptr_t msg) {
  printf("[ERR] %s", msg);
  context->err_count++;
}

cam_int_t wil_lexer_is_at_end(const wil_lexer_context_t *context) {
  return (context->current - context->source) >= strlen(context->source);
}

char wil_lexer_advance(wil_lexer_context_t *context) {
  char c = context->current[0];
  context->current++;
  return c;
}

char wil_lexer_peek(wil_lexer_context_t *context) {
  char c = context->current[0];
  return c;
}

char wil_lexer_peek_next(wil_lexer_context_t *context) {
  context->current++;
  if (wil_lexer_is_at_end(context)) {
    context->current--;
    return '\0';
  }
  context->current--;
  return context->current[1];
}

cam_int_t wil_lexer_match(wil_lexer_context_t *context, char expected) {
  if (wil_lexer_is_at_end(context))
    return CAM_FALSE;
  char c = context->current[0];
  if (c != expected)
    return CAM_FALSE;

  // Advance
  context->current++;
  return CAM_TRUE;
}

cam_int_t wil_lexer_match_i(wil_lexer_context_t *context, char expected) {
  if (wil_lexer_is_at_end(context))
    return CAM_FALSE;
  char c = context->current[0];
  if (c == expected)
    return CAM_TRUE;

  // Advance
  context->current++;
  return CAM_FALSE;
}

cam_out_t wil_lexer_add_token_simple(wil_lexer_context_t *context,
                                     const wil_lexer_token_type_t type) {
  wil_lexer_token_t token = {
      .type = type,
      .lexeme = CAM_STR_NULL,
      .line = context->line,
  };

  CAM_ERR_FAIL_ON_FAIL(cam_type_push_dyn_arr(&context->tokens, &token));
  CAM_ERR_RETURN_SUCCESS();
}

cam_out_t wil_lexer_add_token(wil_lexer_context_t *context,
                              const wil_lexer_token_type_t type) {
  wil_lexer_token_t token = {
      .type = type,
      .lexeme = {.str = context->start,
                 .len = context->current - context->start},
      .line = context->line,
  };

  CAM_ERR_FAIL_ON_FAIL(cam_type_push_dyn_arr(&context->tokens, &token));
  CAM_ERR_RETURN_SUCCESS();
}

cam_out_t wil_lexer_add_token_literal(wil_lexer_context_t *context,
                                      const wil_lexer_token_type_t type,
                                      const wil_lexer_literal_t literal) {
  wil_lexer_token_t token = {
      .type = type,
      .lexeme = {.str = context->start,
                 .len = context->current - context->start},
      .literal = literal,
      .line = context->line,
  };

  CAM_ERR_FAIL_ON_FAIL(cam_type_push_dyn_arr(&context->tokens, &token));
  CAM_ERR_RETURN_SUCCESS();
}

void wil_lexer_identifier(wil_lexer_context_t *context) {
  // Consume all alphanumeric characters in the identifier (which may be a
  // keyword at this point)
  while (cam_type_is_alphanumeric(wil_lexer_peek(context)))
    wil_lexer_advance(context);

  wil_lexer_token_type_t type;
  type = wil_lexer_identifier_is_keyword(context);

  wil_lexer_add_token(context, type);
}
