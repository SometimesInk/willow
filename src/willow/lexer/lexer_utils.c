#include <camellia/camellia.h>
#include <camellia/err/err.h>
#include <camellia/type/dynamic_array.h>
#include <camellia/type/string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <willow/err/err.h>
#include <willow/lexer/lexer.h>
#include <willow/lexer/lexer_utils.h>
#include <willow/lexer/source.h>

void wil_lexer_token_dump(const wil_lexer_token_t *token) {
  printf("Token Dump:\n");
  printf("->Type: %d\n", token->type);
  printf("->Lexeme Len: %zu\n", token->lexeme.len);
  printf("->Lexeme Str: %s\n", token->lexeme.str);
  printf("->Line: %d\n", token->line);
  if (token->type == WIL_LEXER_TOKEN_STRING) {
    printf("->Literal Len: %zu\n", token->lit.s.len);
    printf("->Literal Str: %s\n", token->lit.s.str);
  }
  if (token->type == WIL_LEXER_TOKEN_INTEGER) {
    printf("->Integer: %d\n", token->lit.i);
  }
  if (token->type == WIL_LEXER_TOKEN_DECIMAL) {
    printf("->Double: %f\n", token->lit.d);
  }
}

wil_lexer_sspan_t wil_lexer_construct_sspan(const wil_lexer_context_t *c) {
  return (wil_lexer_sspan_t){.str = c->start, .len = c->current - c->start};
}

cam_int_t wil_lexer_is_at_end(const wil_lexer_context_t *c) {
  return (c->current - c->src.sbuf.str) >= strlen(c->src.sbuf.str);
}

char wil_lexer_advance(wil_lexer_context_t *c) {
  char ch = c->current[0];
  c->current++;
  return ch;
}

char wil_lexer_peek(wil_lexer_context_t *c) {
  char ch = c->current[0];
  return ch;
}

char wil_lexer_peek_next(wil_lexer_context_t *c) {
  c->current++;
  if (wil_lexer_is_at_end(c)) {
    c->current--;
    return '\0';
  }
  c->current--;
  return c->current[1];
}

cam_int_t wil_lexer_match(wil_lexer_context_t *c, const char expected) {
  if (wil_lexer_is_at_end(c))
    return CAM_FALSE;
  char ch = c->current[0];
  if (ch != expected)
    return CAM_FALSE;

  // Advance
  c->current++;
  return CAM_TRUE;
}

cam_int_t wil_lexer_match_i(wil_lexer_context_t *c, const char expected) {
  if (wil_lexer_is_at_end(c))
    return CAM_FALSE;
  char ch = c->current[0];
  if (ch == expected)
    return CAM_TRUE;

  // Advance
  c->current++;
  return CAM_FALSE;
}

//<editor-fold desc="Token Creation">

cam_out_t wil_lexer_add_token_simple(wil_lexer_context_t *c,
                                     const wil_lexer_token_type_t type) {
  wil_lexer_token_t token = {
      .type = type,
      .lexeme = CAM_STR_NULL,
      .line = c->line,
  };

  CAM_ERR_FAIL_ON_FAIL(cam_type_push_dyn_arr(&c->tokens, &token));
  CAM_ERR_RETURN_SUCCESS();
}

cam_out_t wil_lexer_add_token(wil_lexer_context_t *c,
                              const wil_lexer_token_type_t type) {
  wil_lexer_token_t token = {
      .type = type,
      .lexeme = wil_lexer_construct_sspan(c),
      .line = c->line,
  };

  CAM_ERR_FAIL_ON_FAIL(cam_type_push_dyn_arr(&c->tokens, &token));
  CAM_ERR_RETURN_SUCCESS();
}

cam_out_t wil_lexer_add_token_lit(wil_lexer_context_t *c,
                                  const wil_lexer_token_type_t type,
                                  const wil_lexer_literal_t lit) {
  wil_lexer_token_t token = {
      .type = type,
      .lexeme = wil_lexer_construct_sspan(c),
      .lit = lit,
      .line = c->line,
  };

  CAM_ERR_FAIL_ON_FAIL(cam_type_push_dyn_arr(&c->tokens, &token));
  CAM_ERR_RETURN_SUCCESS();
}

//<editor-fold/>

void wil_lexer_identifier(wil_lexer_context_t *c) {
  // Consume all alphanumeric characters in the identifier (which may be a
  // keyword at this point)
  while (cam_type_is_alphanumeric(wil_lexer_peek(c)))
    wil_lexer_advance(c);

  wil_lexer_token_type_t type;
  type = wil_lexer_identifier_is_keyword(c);

  wil_lexer_add_token(c, type);
}

wil_lexer_token_type_t wil_lexer_keyword_check(cam_str_t lit, cam_cptr_t check,
                                               wil_lexer_token_type_t type) {
  printf("Checking whether '%s' matches '%s' for type '%d'\n", lit.str, check,
         type);
  return (cam_type_str_match(lit.str, check, strlen(check)))
             ? type
             : WIL_LEXER_TOKEN_IDENTIFIER;
}

wil_lexer_token_type_t wil_lexer_identifier_is_keyword(wil_lexer_context_t *c) {
  wil_lexer_token_type_t out = WIL_LEXER_TOKEN_IDENTIFIER;

  // Get text literal
  cam_str_t lit = {};
  // TODO: Voiding substring will definitely cause trouble later
  // TODO: Check at least MEMALLOC
  // TODO: Add error signature to cam_type_substring docs
  (void)cam_type_substring(&lit, c->start, 0, c->current - c->start - 1);

#define X(token_type, token_string)                                            \
  do {                                                                         \
    wil_lexer_token_type_t type =                                              \
        wil_lexer_keyword_check(lit, token_string, token_type);                \
    if (type != WIL_LEXER_TOKEN_IDENTIFIER)                                    \
      out = type;                                                              \
  } while (0);
#include <willow/lexer/keyword_definitions.inc>
#undef X

  free(lit.str);
  return out;
}

CAM_STATIC void wil_lexer_parse_dec_(wil_lexer_context_t *c) {
  // Consume digits past decimal
  while (cam_type_is_digit(wil_lexer_peek(c))) {
    if (wil_lexer_is_at_end(c))
      break;
    wil_lexer_advance(c);
  }

  errno = 0;
  double val = strtod(c->start, CAM_NULL);
  if (errno != 0) {
    wil_err_emit_s(c->err, WIL_ERR_CODE_DECIMAL_FORMAT,
                   wil_lexer_construct_sspan(c));
  }

  wil_lexer_add_token_literal(c, WIL_LEXER_TOKEN_DECIMAL,
                              (wil_lexer_literal_t){.d = val});
}

void wil_lexer_lit_num(wil_lexer_context_t *c) {
  // Consume all digits
  while (cam_type_is_digit(wil_lexer_peek(c))) {
    if (wil_lexer_is_at_end(c))
      break;
    wil_lexer_advance(c);
  }

  // Parse decimal
  if (wil_lexer_peek(c) == '.') {
    wil_lexer_advance(c);

    wil_lexer_parse_dec_(c);
    return;
  }

  // Parse as integer
  errno = 0;
  int val = strtol(c->start, CAM_NULL, 10);
  if (errno != 0)
    wil_err_emit_s(c->err, WIL_ERR_CODE_INTEGER_FORMAT,
                   wil_lexer_construct_sspan(c));

  wil_lexer_add_token_literal(c, WIL_LEXER_TOKEN_INTEGER,
                              (wil_lexer_literal_t){.i = val});
}

void wil_lexer_lit_str(wil_lexer_context_t *c) {
  while (c->current[0] != '"' && !wil_lexer_is_at_end(c)) {
    if (wil_lexer_peek(c) == '\n')
      c->line++;
    wil_lexer_advance(c);
  }

  if (wil_lexer_is_at_end(c)) {
    wil_err_emit_s(c->err, WIL_ERR_CODE_UNTERMINATED_STRING,
                   wil_lexer_construct_sspan(c));
    return;
  }

  wil_lexer_advance(c);

  cam_str_t lit;

  // TODO: When creating the literal, here is where to parse escape sequences.

  // -2 for both quotations, -1 to compensate for the previous advance
  cam_size_t end = (cam_size_t)(c->current - c->start - 2 - 1);
  cam_type_substring(&lit, c->start + 1, 0, end); // +1 for quote

  wil_lexer_add_token_literal(c, WIL_LEXER_TOKEN_STRING,
                              (wil_lexer_literal_t){.s = lit});
}
