#include <camellia/camellia.h>
#include <camellia/err/err.h>
#include <camellia/type/dynamic_array.h>
#include <camellia/type/string.h>
#include <stdio.h>
#include <stdlib.h>
#include <willow/err/err.h>
#include <willow/lexer/lexer.h>
#include <willow/lexer/lexer_utils.h>

wil_lexer_context_t wil_lexer_create_context(wil_err_context_t *err,
                                             const cam_cptr_t src) {
  wil_lexer_context_t c = {
      .err = err,
      .src = cam_create_str(src),

      .line = 1,

      .start = src,
      .current = src,
  };
  cam_type_create_dyn_arr(&c.tokens, sizeof(wil_lexer_token_t),
                          WIL_LEXER_TOKEN_ARR_INI_LEN);
  return c;
}

void wil_lexer_dispose_context(wil_lexer_context_t *context) {
  if (context == (wil_lexer_context_t *)CAM_NULL)
    return;
  if (&context->tokens == (cam_type_dyn_arr_t *)CAM_NULL)
    return;

  // Free strings from tokens
  for (size_t i = 0; i < cam_type_len_dyn_arr(&context->tokens); i++) {
    wil_lexer_token_t *token =
        (wil_lexer_token_t *)cam_type_get_dyn_arr(&context->tokens, i);

    if (token->type == WIL_LEXER_TOKEN_STRING)
      free(token->lit.s.str);
  }

  cam_type_free_dyn_arr(&context->tokens);
}

cam_out_t wil_lexer_scan_tokens(wil_lexer_context_t *c) {
  while (!wil_lexer_is_at_end(c)) {
    c->start = c->current;
    if (wil_lexer_scan_one_token(c) == CAM_FAILURE)
      return CAM_FAILURE;
  }

  if (wil_lexer_add_token_simple(c, WIL_LEXER_TOKEN_EOF) == CAM_FAILURE)
    return CAM_FAILURE;
  CAM_ERR_RETURN_SUCCESS();
}
cam_out_t wil_lexer_scan_one_token(wil_lexer_context_t *c) {
  char ch = wil_lexer_advance(c);
  switch (ch) {
  case '(':
    wil_lexer_add_token(c, WIL_LEXER_TOKEN_LEFT_PAREN);
    break;
  case ')':
    wil_lexer_add_token(c, WIL_LEXER_TOKEN_RIGHT_PAREN);
    break;
  case '{':
    wil_lexer_add_token(c, WIL_LEXER_TOKEN_LEFT_BRACE);
    break;
  case '}':
    wil_lexer_add_token(c, WIL_LEXER_TOKEN_RIGHT_BRACE);
    break;
  case ',':
    wil_lexer_add_token(c, WIL_LEXER_TOKEN_COMMA);
    break;
  case '.':
    wil_lexer_add_token(c, WIL_LEXER_TOKEN_DOT);
    break;
  case '-':
    wil_lexer_add_token(c, WIL_LEXER_TOKEN_MINUS);
    break;
  case '+':
    wil_lexer_add_token(c, WIL_LEXER_TOKEN_PLUS);
    break;
  case ';':
    wil_lexer_add_token(c, WIL_LEXER_TOKEN_SEMICOLON);
    break;
  case '*':
    wil_lexer_add_token(c, WIL_LEXER_TOKEN_STAR);
    break;
  case '!':
    wil_lexer_add_token(c, wil_lexer_match(c, '=') ? WIL_LEXER_TOKEN_BANG_EQUAL
                                                   : WIL_LEXER_TOKEN_BANG);
    break;
  case '=':
    wil_lexer_add_token(c, wil_lexer_match(c, '=') ? WIL_LEXER_TOKEN_EQUAL_EQUAL
                                                   : WIL_LEXER_TOKEN_EQUAL);
    break;
  case '<':
    wil_lexer_add_token(c, wil_lexer_match(c, '=') ? WIL_LEXER_TOKEN_LESS_EQUAL
                                                   : WIL_LEXER_TOKEN_LESS);
    break;
  case '>':
    wil_lexer_add_token(c, wil_lexer_match(c, '=')
                               ? WIL_LEXER_TOKEN_GREATER_EQUAL
                               : WIL_LEXER_TOKEN_GREATER);
    break;
  case '/':
    if (wil_lexer_match(c, '/')) {
      // Skip rest of line on comment
      while (wil_lexer_peek(c) != '\n' && !wil_lexer_is_at_end(c))
        wil_lexer_advance(c);
    } else {
      wil_lexer_add_token(c, WIL_LEXER_TOKEN_SLASH);
    }
    break;
  case ' ':
  case '\r':
  case '\t':
    // Ignore whitespace
    break;
  case '\n':
    c->line++;
    break;
  case '"':
    wil_lexer_lit_str(c);
    break;
  default:
    if (cam_type_is_digit(ch)) {
      wil_lexer_lit_num(c);
    } else if (cam_type_is_alpha(ch)) {
      wil_lexer_identifier(c);
    } else {
      wil_err_emit_s(c->err, WIL_ERR_CODE_INVALID_CHAR,
                     wil_lexer_construct_sspan(c));
    }
    break;
  }

  CAM_ERR_RETURN_SUCCESS();
}
