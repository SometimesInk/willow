#include <camellia/err/err.h>
#include <willow/lexer/lexer_utils.h>
#include <willow/lexer/tokens.h>

cam_out_t wil_lexer_scan_one_token(wil_lexer_context_t *context,
                                   cam_type_dyn_arr_t *arr) {
  // TODO:

  char c = wil_lexer_advance(context);
  switch (c) {
  case '(':
    wil_lexer_add_token_simple(context, WIL_LEXER_TOKEN_LEFT_PAREN);
  case ')':
    wil_lexer_add_token_simple(context, WIL_LEXER_TOKEN_RIGHT_PAREN);
  case '{':
    wil_lexer_add_token_simple(context, WIL_LEXER_TOKEN_LEFT_BRACE);
  case '}':
    wil_lexer_add_token_simple(context, WIL_LEXER_TOKEN_RIGHT_BRACE);
  case ',':
    wil_lexer_add_token_simple(context, WIL_LEXER_TOKEN_COMMA);
  case '.':
    wil_lexer_add_token_simple(context, WIL_LEXER_TOKEN_DOT);
  case '-':
    wil_lexer_add_token_simple(context, WIL_LEXER_TOKEN_MINUS);
  case '+':
    wil_lexer_add_token_simple(context, WIL_LEXER_TOKEN_PLUS);
  case ';':
    wil_lexer_add_token_simple(context, WIL_LEXER_TOKEN_SEMICOLON);
  case '*':
    wil_lexer_add_token_simple(context, WIL_LEXER_TOKEN_STAR);
  case '!':
    wil_lexer_add_token_simple(context, wil_lexer_match(context, '=')
                                            ? WIL_LEXER_TOKEN_BANG_EQUAL
                                            : WIL_LEXER_TOKEN_BANG);
  case '=':
    wil_lexer_add_token_simple(context, wil_lexer_match(context, '=')
                                            ? WIL_LEXER_TOKEN_EQUAL_EQUAL
                                            : WIL_LEXER_TOKEN_EQUAL);
  case '<':
    wil_lexer_add_token_simple(context, wil_lexer_match(context, '=')
                                            ? WIL_LEXER_TOKEN_LESS_EQUAL
                                            : WIL_LEXER_TOKEN_LESS);
  case '>':
    wil_lexer_add_token_simple(context, wil_lexer_match(context, '=')
                                            ? WIL_LEXER_TOKEN_GREATER_EQUAL
                                            : WIL_LEXER_TOKEN_GREATER);
  default:
    wil_lexer_err(context, "Invalid character."); // TODO: Use an enum for these
  }

  CAM_ERR_RETURN_SUCCESS();
}
