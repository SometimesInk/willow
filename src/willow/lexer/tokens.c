#include <camellia/err/err.h>
#include <camellia/type/string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <willow/lexer/lexer_utils.h>
#include <willow/lexer/tokens.h>

cam_out_t wil_lexer_scan_one_token(wil_lexer_context_t *context,
                                   cam_type_dyn_arr_t *arr) {
  // TODO:

  char c = wil_lexer_advance(context);
  switch (c) {
  case '(':
    wil_lexer_add_token(context, WIL_LEXER_TOKEN_LEFT_PAREN);
    break;
  case ')':
    wil_lexer_add_token(context, WIL_LEXER_TOKEN_RIGHT_PAREN);
    break;
  case '{':
    wil_lexer_add_token(context, WIL_LEXER_TOKEN_LEFT_BRACE);
    break;
  case '}':
    wil_lexer_add_token(context, WIL_LEXER_TOKEN_RIGHT_BRACE);
    break;
  case ',':
    wil_lexer_add_token(context, WIL_LEXER_TOKEN_COMMA);
    break;
  case '.':
    wil_lexer_add_token(context, WIL_LEXER_TOKEN_DOT);
    break;
  case '-':
    wil_lexer_add_token(context, WIL_LEXER_TOKEN_MINUS);
    break;
  case '+':
    wil_lexer_add_token(context, WIL_LEXER_TOKEN_PLUS);
    break;
  case ';':
    wil_lexer_add_token(context, WIL_LEXER_TOKEN_SEMICOLON);
    break;
  case '*':
    wil_lexer_add_token(context, WIL_LEXER_TOKEN_STAR);
    break;
  case '!':
    wil_lexer_add_token(context, wil_lexer_match(context, '=')
                                     ? WIL_LEXER_TOKEN_BANG_EQUAL
                                     : WIL_LEXER_TOKEN_BANG);
    break;
  case '=':
    wil_lexer_add_token(context, wil_lexer_match(context, '=')
                                     ? WIL_LEXER_TOKEN_EQUAL_EQUAL
                                     : WIL_LEXER_TOKEN_EQUAL);
    break;
  case '<':
    wil_lexer_add_token(context, wil_lexer_match(context, '=')
                                     ? WIL_LEXER_TOKEN_LESS_EQUAL
                                     : WIL_LEXER_TOKEN_LESS);
    break;
  case '>':
    wil_lexer_add_token(context, wil_lexer_match(context, '=')
                                     ? WIL_LEXER_TOKEN_GREATER_EQUAL
                                     : WIL_LEXER_TOKEN_GREATER);
    break;
  case '/':
    if (wil_lexer_match(context, '/')) {
      // Skip rest of line on comment
      while (wil_lexer_peek(context) != '\n' && !wil_lexer_is_at_end(context))
        wil_lexer_advance(context);
    } else {
      wil_lexer_add_token(context, WIL_LEXER_TOKEN_SLASH);
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
    if (cam_type_is_digit(c)) {
      wil_lexer_integer_and_float_literal(context);
    } else if (cam_type_is_alpha(c)) {
      wil_lexer_identifier(context);
    } else {
      wil_lexer_err(context, "Invalid character.");
    }
    break;
  }

  CAM_ERR_RETURN_SUCCESS();
}

void wil_lexer_token_dump(const wil_lexer_token_t *token) {
  if (token->type == WIL_LEXER_TOKEN_STRING) {
    wil_lexer_token_dump_str(token);
    return;
  }
  if (token->type == WIL_LEXER_TOKEN_INTEGER) {
    wil_lexer_token_dump_int(token);
    return;
  }
  if (token->type == WIL_LEXER_TOKEN_FLOAT) {
    wil_lexer_token_dump_dec(token);
    return;
  }
  wil_lexer_token_dump_none(token);
}

void wil_lexer_token_dump_none(const wil_lexer_token_t *token) {
  printf("Token Dump:\n");
  printf("->Type: %d\n", token->type);
  printf("->Lexeme Len: %zu\n", token->lexeme.len);
  printf("->Lexeme Str: %s\n", token->lexeme.str);
  printf("->Line: %d\n", token->line);
}

void wil_lexer_token_dump_str(const wil_lexer_token_t *token) {
  wil_lexer_token_dump_none(token);
  printf("->Literal Len: %zu\n", token->literal.string.len);
  printf("->Literal Str: %s\n", token->literal.string.str);
}

void wil_lexer_token_dump_int(const wil_lexer_token_t *token) {
  wil_lexer_token_dump_none(token);
  printf("->Integer: %d\n", token->literal.integer);
}

void wil_lexer_token_dump_dec(const wil_lexer_token_t *token) {
  wil_lexer_token_dump_none(token);
  printf("->Decimal: %f\n", token->literal.decimal);
}

wil_lexer_token_type_t wil_lexer_keyword_check(cam_str_t lit, cam_cptr_t check,
                                               wil_lexer_token_type_t type) {
  printf("Checking whether '%s' matches '%s' for type '%d'\n", lit.str, check,
         type);
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
                           context->current - context->start - 1);

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

void wil_lexer_parse_float(wil_lexer_context_t *context) {
  // Consume digits past decimal
  while (cam_type_is_digit(wil_lexer_peek(context))) {
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
  while (cam_type_is_digit(wil_lexer_peek(context))) {
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

  // TODO: When creating the literal, here is where to parse escape sequences.

  // -2 for both quotations, -1 to compensate for the previous advance
  cam_size_t end = (cam_size_t)(context->current - context->start - 2 - 1);
  cam_type_substring(&literal, context->start + 1, 0, end); // +1 for quote

  wil_lexer_add_token_literal(context, WIL_LEXER_TOKEN_STRING,
                              (wil_lexer_literal_t){.string = literal});
}
