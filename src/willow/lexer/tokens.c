#include <camellia/err/err.h>
#include <willow/lexer/lexer_utils.h>
#include <willow/lexer/string_lexer.h>
#include <willow/lexer/tokens.h>

cam_out_t wil_lexer_scan_one_token(wil_lexer_context_t *context,
                                   cam_type_dyn_arr_t *arr) {
  // TODO:

  char c = wil_lexer_advance(context);
  switch (c) {
  case '(':
    wil_lexer_add_token_simple(context, WIL_LEXER_TOKEN_LEFT_PAREN);
    break;
  case ')':
    wil_lexer_add_token_simple(context, WIL_LEXER_TOKEN_RIGHT_PAREN);
    break;
  case '{':
    wil_lexer_add_token_simple(context, WIL_LEXER_TOKEN_LEFT_BRACE);
    break;
  case '}':
    wil_lexer_add_token_simple(context, WIL_LEXER_TOKEN_RIGHT_BRACE);
    break;
  case ',':
    wil_lexer_add_token_simple(context, WIL_LEXER_TOKEN_COMMA);
    break;
  case '.':
    wil_lexer_add_token_simple(context, WIL_LEXER_TOKEN_DOT);
    break;
  case '-':
    wil_lexer_add_token_simple(context, WIL_LEXER_TOKEN_MINUS);
    break;
  case '+':
    wil_lexer_add_token_simple(context, WIL_LEXER_TOKEN_PLUS);
    break;
  case ';':
    wil_lexer_add_token_simple(context, WIL_LEXER_TOKEN_SEMICOLON);
    break;
  case '*':
    wil_lexer_add_token_simple(context, WIL_LEXER_TOKEN_STAR);
    break;
  case '!':
    wil_lexer_add_token_simple(context, wil_lexer_match(context, '=')
                                            ? WIL_LEXER_TOKEN_BANG_EQUAL
                                            : WIL_LEXER_TOKEN_BANG);
    break;
  case '=':
    wil_lexer_add_token_simple(context, wil_lexer_match(context, '=')
                                            ? WIL_LEXER_TOKEN_EQUAL_EQUAL
                                            : WIL_LEXER_TOKEN_EQUAL);
    break;
  case '<':
    wil_lexer_add_token_simple(context, wil_lexer_match(context, '=')
                                            ? WIL_LEXER_TOKEN_LESS_EQUAL
                                            : WIL_LEXER_TOKEN_LESS);
    break;
  case '>':
    wil_lexer_add_token_simple(context, wil_lexer_match(context, '=')
                                            ? WIL_LEXER_TOKEN_GREATER_EQUAL
                                            : WIL_LEXER_TOKEN_GREATER);
    break;
  case '/':
    if (wil_lexer_match(context, '/')) {
      // Skip rest of line on comment
      while (wil_lexer_peek(context) != '\n' && !wil_lexer_is_at_end(context))
        wil_lexer_advance(context);
    } else {
      wil_lexer_add_token_simple(context, WIL_LEXER_TOKEN_SLASH);
    }
    break;
  case ' ':
  case '\r':
  case '\t':
    // Ignore whitespace
    break;
  case '\n':
    context->line++;
    break;
  case '"':
    wil_lexer_string_literal(context);
    break;
  default:
    // TODO: Use an enum for these errors
    wil_lexer_err(context, "Invalid character.");
    break;
  }

  CAM_ERR_RETURN_SUCCESS();
}
