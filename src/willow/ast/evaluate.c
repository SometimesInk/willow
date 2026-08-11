#include <camellia/camellia.h>
#include <camellia/type/string.h>
#include <stdio.h>
#include <willow/ast/evaluate.h>
#include <willow/ast/expressions.h>
#include <willow/err/err.h>
#include <willow/lexer/lexer.h>

CAM_STATIC cam_int_t wil_ast_numeral_(wil_ast_value_t val) {
  return val.type == WIL_AST_VALUE_TYPE_INTEGER ||
         val.type == WIL_AST_VALUE_TYPE_DECIMAL;
}

CAM_STATIC cam_int_t wil_ast_both_numerals_(wil_ast_value_t a,
                                            wil_ast_value_t b) {
  return wil_ast_numeral_(a) && wil_ast_numeral_(b);
}

CAM_STATIC cam_int_t wil_ast_truthiness_(wil_ast_value_t val) {
  switch (val.type) {
  case WIL_AST_VALUE_TYPE_BOOLEAN:
    return val.val.b;
  case WIL_AST_VALUE_TYPE_STRING:
    if (val.val.s.len == 0)
      return CAM_FALSE;
    break;
  case WIL_AST_VALUE_TYPE_NULL:
    return CAM_FALSE;
  case WIL_AST_VALUE_TYPE_CHAR:
    if (val.val.c == 0)
      return CAM_FALSE;
    break;
  case WIL_AST_VALUE_TYPE_INTEGER:
    if (val.val.i == 0)
      return CAM_FALSE;
    break;
  case WIL_AST_VALUE_TYPE_DECIMAL:
    if (val.val.d == 0.0)
      return CAM_FALSE;
    break;
  }
  return CAM_TRUE;
}

CAM_STATIC cam_int_t wil_ast_equal_(wil_ast_interpreter_context_t *c,
                                    wil_ast_value_t a, wil_ast_value_t b,
                                    wil_ast_expr_t *expr) {
  if (a.type != b.type) {
    wil_err_emit_e(c->err, WIL_ERR_CODE_EQUAL_TYPES_DIFFER, expr);
    return CAM_FALSE;
  }
  switch (a.type) {
  case WIL_AST_VALUE_TYPE_DECIMAL:
    return a.val.d == b.val.d;
    break;
  case WIL_AST_VALUE_TYPE_INTEGER:
    return a.val.i == b.val.i;
    break;
  case WIL_AST_VALUE_TYPE_STRING:
    return a.val.s.len == b.val.s.len &&
           cam_type_str_match(a.val.s.str, b.val.s.str, a.val.s.len);
    break;
  case WIL_AST_VALUE_TYPE_CHAR:
    return a.val.c == b.val.c;
    break;
  case WIL_AST_VALUE_TYPE_BOOLEAN:
    return a.val.b == b.val.b;
    break;
  case WIL_AST_VALUE_TYPE_NULL:
    return CAM_TRUE;
    break;
  }
  return CAM_TRUE;
}

CAM_STATIC wil_ast_value_t wil_ast_eval_lit_(wil_ast_interpreter_context_t *c,
                                             wil_ast_expr_t *expr) {
  switch (expr->lit.token.type) {
  case WIL_LEXER_TOKEN_KW_FALSE:
    return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_BOOLEAN,
                             .val.b = CAM_FALSE};
  case WIL_LEXER_TOKEN_KW_TRUE:
    return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_BOOLEAN,
                             .val.b = CAM_TRUE};
  case WIL_LEXER_TOKEN_KW_NULL:
    return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_NULL};
  case WIL_LEXER_TOKEN_INTEGER:
    return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_INTEGER,
                             .val.i = expr->lit.token.lit.i};
  case WIL_LEXER_TOKEN_STRING:
    return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_STRING,
                             .val.s = expr->lit.token.lit.s};
  case WIL_LEXER_TOKEN_DECIMAL:
    return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_DECIMAL,
                             .val.d = expr->lit.token.lit.d};
  }
  wil_err_emit_e(c->err, WIL_ERR_CODE_LIT_TYPE, expr,
                 wil_lexer_token_type_name[expr->lit.token.type]);
  return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_NONE};
}

CAM_STATIC wil_ast_value_t wil_ast_eval_group_(wil_ast_interpreter_context_t *c,
                                               wil_ast_expr_t *expr) {
  return wil_ast_eval_expr(c, expr->group.expr);
}

CAM_STATIC wil_ast_value_t
wil_ast_eval_unary_minus_(wil_ast_interpreter_context_t *c,
                          wil_ast_value_t right, wil_ast_expr_t *expr) {
  switch (right.type) {
  case WIL_AST_VALUE_TYPE_INTEGER:
    return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_INTEGER,
                             .val.i = -right.val.i};
  case WIL_AST_VALUE_TYPE_DECIMAL:
    return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_DECIMAL,
                             .val.d = -right.val.d};
  default:
    wil_err_emit_e(c->err, WIL_ERR_CODE_UNARY_MINUS_TYPE, expr);
    return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_NONE};
  }
}

CAM_STATIC wil_ast_value_t wil_ast_eval_unary_(wil_ast_interpreter_context_t *c,
                                               wil_ast_expr_t *expr) {
  wil_ast_value_t right = wil_ast_eval_expr(c, expr->unary.expr);
  switch (expr->unary.op->type) {
  case WIL_LEXER_TOKEN_BANG:
    return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_BOOLEAN,
                             .val.b = !wil_ast_truthiness_(right)};
  case WIL_LEXER_TOKEN_MINUS:
    return wil_ast_eval_unary_minus_(c, right, expr);
  }
  wil_err_emit_e(c->err, WIL_ERR_CODE_UNARY_TYPE, expr,
                 wil_lexer_token_type_name[expr->unary.op->type]);
  return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_NONE};
}

CAM_STATIC wil_ast_value_t
wil_ast_eval_binary_slash_(wil_ast_interpreter_context_t *c, wil_ast_value_t a,
                           wil_ast_value_t b, wil_ast_expr_t *expr) {
  if (!wil_ast_both_numerals_(a, b)) {
    wil_err_emit_e(c->err, WIL_ERR_CODE_BINARY_SLASH_TYPE, expr);
    return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_NONE};
  }
  if (a.type == WIL_AST_VALUE_TYPE_INTEGER &&
      b.type == WIL_AST_VALUE_TYPE_INTEGER) {
    if (b.val.i == 0) {
      wil_err_emit_e(c->err, WIL_ERR_CODE_DIV_ZERO, expr);
      return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_NONE};
    }
    return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_INTEGER,
                             .val.i = a.val.i / b.val.i};
  }
  float lhs = a.type == WIL_AST_VALUE_TYPE_DECIMAL ? a.val.d : (float)a.val.i;
  float rhs = b.type == WIL_AST_VALUE_TYPE_DECIMAL ? b.val.d : (float)b.val.i;
  if (rhs == 0.0) {
    wil_err_emit_e(c->err, WIL_ERR_CODE_DIV_ZERO, expr);
    return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_NONE};
  }
  return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_DECIMAL,
                           .val.d = lhs / rhs};
}

CAM_STATIC wil_ast_value_t
wil_ast_eval_binary_star_(wil_ast_interpreter_context_t *c, wil_ast_value_t a,
                          wil_ast_value_t b, wil_ast_expr_t *expr) {
  if (!wil_ast_both_numerals_(a, b)) {
    wil_err_emit_e(c->err, WIL_ERR_CODE_BINARY_STAR_TYPE, expr);
    return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_NONE};
  }
  if (a.type == WIL_AST_VALUE_TYPE_INTEGER &&
      b.type == WIL_AST_VALUE_TYPE_INTEGER) {
    return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_INTEGER,
                             .val.i = a.val.i * b.val.i};
  }
  float lhs = a.type == WIL_AST_VALUE_TYPE_DECIMAL ? a.val.d : (float)a.val.i;
  float rhs = b.type == WIL_AST_VALUE_TYPE_DECIMAL ? b.val.d : (float)b.val.i;
  return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_DECIMAL,
                           .val.d = lhs * rhs};
}

CAM_STATIC wil_ast_value_t
wil_ast_eval_binary_minus_(wil_ast_interpreter_context_t *c, wil_ast_value_t a,
                           wil_ast_value_t b, wil_ast_expr_t *expr) {
  if (!wil_ast_both_numerals_(a, b)) {
    wil_err_emit_e(c->err, WIL_ERR_CODE_BINARY_MINUS_TYPE, expr);
    return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_NONE};
  }
  if (a.type == WIL_AST_VALUE_TYPE_INTEGER &&
      b.type == WIL_AST_VALUE_TYPE_INTEGER) {
    return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_INTEGER,
                             .val.i = a.val.i - b.val.i};
  }
  float lhs = a.type == WIL_AST_VALUE_TYPE_DECIMAL ? a.val.d : (float)a.val.i;
  float rhs = b.type == WIL_AST_VALUE_TYPE_DECIMAL ? b.val.d : (float)b.val.i;
  return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_DECIMAL,
                           .val.d = lhs - rhs};
}

CAM_STATIC wil_ast_value_t
wil_ast_eval_binary_plus_(wil_ast_interpreter_context_t *c, wil_ast_value_t a,
                          wil_ast_value_t b, wil_ast_expr_t *expr) {
  if (!wil_ast_both_numerals_(a, b)) {
    wil_err_emit_e(c->err, WIL_ERR_CODE_BINARY_PLUS_TYPE, expr);
    return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_NONE};
  }
  if (a.type == WIL_AST_VALUE_TYPE_INTEGER &&
      b.type == WIL_AST_VALUE_TYPE_INTEGER) {
    return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_INTEGER,
                             .val.i = a.val.i + b.val.i};
  }
  float lhs = a.type == WIL_AST_VALUE_TYPE_DECIMAL ? a.val.d : (float)a.val.i;
  float rhs = b.type == WIL_AST_VALUE_TYPE_DECIMAL ? b.val.d : (float)b.val.i;
  return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_DECIMAL,
                           .val.d = lhs + rhs};
}

CAM_STATIC wil_ast_value_t wil_ast_eval_binary_greater_(
    wil_ast_interpreter_context_t *c, wil_ast_value_t a, wil_ast_value_t b,
    wil_ast_expr_t *expr) {
  if (a.type == WIL_AST_VALUE_TYPE_INTEGER &&
      b.type == WIL_AST_VALUE_TYPE_INTEGER)
    return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_BOOLEAN,
                             .val.b = a.val.i > b.val.i};
  if (a.type == WIL_AST_VALUE_TYPE_DECIMAL &&
      b.type == WIL_AST_VALUE_TYPE_DECIMAL)
    return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_BOOLEAN,
                             .val.b = a.val.d > b.val.d};
  if (a.type == WIL_AST_VALUE_TYPE_INTEGER &&
      b.type == WIL_AST_VALUE_TYPE_DECIMAL)
    return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_BOOLEAN,
                             .val.b = a.val.i > b.val.d};
  if (a.type == WIL_AST_VALUE_TYPE_DECIMAL &&
      b.type == WIL_AST_VALUE_TYPE_INTEGER)
    return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_BOOLEAN,
                             .val.b = a.val.d > b.val.i};
  wil_err_emit_e(c->err, WIL_ERR_CODE_BINARY_GREATER_TYPE, expr);
  return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_NONE};
}

CAM_STATIC wil_ast_value_t wil_ast_eval_binary_greater_equal_(
    wil_ast_interpreter_context_t *c, wil_ast_value_t a, wil_ast_value_t b,
    wil_ast_expr_t *expr) {
  if (a.type == WIL_AST_VALUE_TYPE_INTEGER &&
      b.type == WIL_AST_VALUE_TYPE_INTEGER)
    return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_BOOLEAN,
                             .val.b = a.val.i >= b.val.i};
  if (a.type == WIL_AST_VALUE_TYPE_DECIMAL &&
      b.type == WIL_AST_VALUE_TYPE_DECIMAL)
    return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_BOOLEAN,
                             .val.b = a.val.d >= b.val.d};
  if (a.type == WIL_AST_VALUE_TYPE_INTEGER &&
      b.type == WIL_AST_VALUE_TYPE_DECIMAL)
    return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_BOOLEAN,
                             .val.b = a.val.i >= b.val.d};
  if (a.type == WIL_AST_VALUE_TYPE_DECIMAL &&
      b.type == WIL_AST_VALUE_TYPE_INTEGER)
    return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_BOOLEAN,
                             .val.b = a.val.d >= b.val.i};
  wil_err_emit_e(c->err, WIL_ERR_CODE_BINARY_GREATER_EQUAL_TYPE, expr);
  return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_NONE};
}

CAM_STATIC wil_ast_value_t
wil_ast_eval_binary_less_(wil_ast_interpreter_context_t *c, wil_ast_value_t a,
                          wil_ast_value_t b, wil_ast_expr_t *expr) {
  if (a.type == WIL_AST_VALUE_TYPE_INTEGER &&
      b.type == WIL_AST_VALUE_TYPE_INTEGER)
    return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_BOOLEAN,
                             .val.b = a.val.i < b.val.i};
  if (a.type == WIL_AST_VALUE_TYPE_DECIMAL &&
      b.type == WIL_AST_VALUE_TYPE_DECIMAL)
    return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_BOOLEAN,
                             .val.b = a.val.d < b.val.d};
  if (a.type == WIL_AST_VALUE_TYPE_INTEGER &&
      b.type == WIL_AST_VALUE_TYPE_DECIMAL)
    return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_BOOLEAN,
                             .val.b = a.val.i < b.val.d};
  if (a.type == WIL_AST_VALUE_TYPE_DECIMAL &&
      b.type == WIL_AST_VALUE_TYPE_INTEGER)
    return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_BOOLEAN,
                             .val.b = a.val.d < b.val.i};
  wil_err_emit_e(c->err, WIL_ERR_CODE_BINARY_LESS_TYPE, expr);
  return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_NONE};
}

CAM_STATIC wil_ast_value_t wil_ast_eval_binary_less_equal_(
    wil_ast_interpreter_context_t *c, wil_ast_value_t a, wil_ast_value_t b,
    wil_ast_expr_t *expr) {
  if (a.type == WIL_AST_VALUE_TYPE_INTEGER &&
      b.type == WIL_AST_VALUE_TYPE_INTEGER)
    return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_BOOLEAN,
                             .val.b = a.val.i <= b.val.i};
  if (a.type == WIL_AST_VALUE_TYPE_DECIMAL &&
      b.type == WIL_AST_VALUE_TYPE_DECIMAL)
    return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_BOOLEAN,
                             .val.b = a.val.d <= b.val.d};
  if (a.type == WIL_AST_VALUE_TYPE_INTEGER &&
      b.type == WIL_AST_VALUE_TYPE_DECIMAL)
    return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_BOOLEAN,
                             .val.b = a.val.i <= b.val.d};
  if (a.type == WIL_AST_VALUE_TYPE_DECIMAL &&
      b.type == WIL_AST_VALUE_TYPE_INTEGER)
    return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_BOOLEAN,
                             .val.b = a.val.d <= b.val.i};
  wil_err_emit_e(c->err, WIL_ERR_CODE_BINARY_LESS_EQUAL_TYPE, expr);
  return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_NONE};
}

CAM_STATIC wil_ast_value_t
wil_ast_eval_binary_(wil_ast_interpreter_context_t *c, wil_ast_expr_t *expr) {
  wil_ast_value_t a = wil_ast_eval_expr(c, expr->binary.left);
  wil_ast_value_t b = wil_ast_eval_expr(c, expr->binary.right);
  switch (expr->binary.op->type) {
  case WIL_LEXER_TOKEN_MINUS:
    return wil_ast_eval_binary_minus_(c, a, b, expr);
  case WIL_LEXER_TOKEN_SLASH:
    return wil_ast_eval_binary_slash_(c, a, b, expr);
  case WIL_LEXER_TOKEN_STAR:
    return wil_ast_eval_binary_star_(c, a, b, expr);
  case WIL_LEXER_TOKEN_PLUS:
    return wil_ast_eval_binary_plus_(c, a, b, expr);
  case WIL_LEXER_TOKEN_GREATER:
    return wil_ast_eval_binary_greater_(c, a, b, expr);
  case WIL_LEXER_TOKEN_GREATER_EQUAL:
    return wil_ast_eval_binary_greater_equal_(c, a, b, expr);
  case WIL_LEXER_TOKEN_LESS:
    return wil_ast_eval_binary_less_(c, a, b, expr);
  case WIL_LEXER_TOKEN_LESS_EQUAL:
    return wil_ast_eval_binary_less_equal_(c, a, b, expr);
  case WIL_LEXER_TOKEN_BANG_EQUAL:
    return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_BOOLEAN,
                             .val.b = !wil_ast_equal_(c, a, b, expr)};
  case WIL_LEXER_TOKEN_EQUAL_EQUAL:
    return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_BOOLEAN,
                             .val.b = wil_ast_equal_(c, a, b, expr)};
  }
  wil_err_emit_e(c->err, WIL_ERR_CODE_BINARY_TYPE, expr,
                 wil_lexer_token_type_name[expr->binary.op->type]);
  return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_NONE};
}

wil_ast_value_t wil_ast_eval_expr(wil_ast_interpreter_context_t *c,
                                  wil_ast_expr_t *expr) {
  switch (expr->type) {
#define X(name, upper, components)                                             \
  case WIL_AST_EXPR_TYPE_##upper:                                              \
    return wil_ast_eval_##name##_(c, expr);
#include <willow/ast/expressions.inc>
#undef X
  }

  // Unreachable
}

void wil_ast_print_eval_value(wil_ast_value_t val) {
  switch (val.type) {
  case WIL_AST_VALUE_TYPE_NONE:
    printf("none\n");
    break;
  case WIL_AST_VALUE_TYPE_BOOLEAN:
    printf(val.val.b == CAM_TRUE ? "true\n" : "false\n");
    break;
  case WIL_AST_VALUE_TYPE_STRING:
    printf("%.*s\n", (cam_int_t)val.val.s.len, val.val.s.str);
    break;
  case WIL_AST_VALUE_TYPE_NULL:
    printf("null\n");
    break;
  case WIL_AST_VALUE_TYPE_CHAR:
    printf("%c\n", val.val.c);
    break;
  case WIL_AST_VALUE_TYPE_INTEGER:
    printf("%d\n", val.val.i);
    break;
  case WIL_AST_VALUE_TYPE_DECIMAL:
    printf("%f\n", val.val.d);
    break;
  }
}
