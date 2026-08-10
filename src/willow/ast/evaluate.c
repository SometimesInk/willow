#include <camellia/camellia.h>
#include <willow/ast/evaluate.h>
#include <willow/ast/expressions.h>
#include <willow/err/err.h>
#include <willow/lexer/tokens.h>

CAM_STATIC cam_int_t wil_ast_truthiness_(wil_ast_value_t val) {
  switch (val.type) {
  case WIL_AST_VALUE_TYPE_BOOLEAN:
    return val.val.boolean;
  case WIL_AST_VALUE_TYPE_STRING:
    if (val.val.string.len == 0)
      return CAM_FALSE;
    break;
  case WIL_AST_VALUE_TYPE_NULL:
    return CAM_FALSE;
  case WIL_AST_VALUE_TYPE_CHAR:
    if (val.val.c == 0)
      return CAM_FALSE;
    break;
  case WIL_AST_VALUE_TYPE_INTEGER:
    if (val.val.integer == 0)
      return CAM_FALSE;
    break;
  case WIL_AST_VALUE_TYPE_FLOAT:
    if (val.val.decimal == 0.0)
      return CAM_FALSE;
    break;
  }
  return CAM_TRUE;
}

CAM_STATIC cam_int_t wil_ast_equal_(wil_ast_value_t a, wil_ast_value_t b) {
  return wil_ast_truthiness_(a) == wil_ast_truthiness_(b);
}

CAM_STATIC wil_ast_value_t wil_ast_eval_lit_(wil_ast_interpreter_context_t *c,
                                             wil_ast_expr_t *expr) {
  wil_ast_value_t out;
  switch (expr->lit.token.type) {
  case WIL_LEXER_TOKEN_KW_FALSE:
    return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_BOOLEAN,
                             .val.boolean = CAM_FALSE};
  case WIL_LEXER_TOKEN_KW_TRUE:
    return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_BOOLEAN,
                             .val.boolean = CAM_TRUE};
  case WIL_LEXER_TOKEN_KW_NULL:
    return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_NULL};
  case WIL_LEXER_TOKEN_INTEGER:
    return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_INTEGER,
                             .val.integer = expr->lit.token.literal.integer};
  case WIL_LEXER_TOKEN_STRING:
    return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_STRING,
                             .val.string = expr->lit.token.literal.string};
  case WIL_LEXER_TOKEN_FLOAT:
    return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_FLOAT,
                             .val.decimal = expr->lit.token.literal.decimal};
  }
  wil_err_emit_e(c->err, WIL_ERR_CODE_LIT_TYPE, expr,
                 wil_lexer_token_type_name[expr->unary.op->type]);
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
                             .val.integer = -right.val.integer};
  case WIL_AST_VALUE_TYPE_FLOAT:
    return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_FLOAT,
                             .val.decimal = -right.val.decimal};
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
                             .val.boolean = !wil_ast_truthiness_(right)};
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
  if ((a.type != WIL_AST_VALUE_TYPE_INTEGER &&
       a.type != WIL_AST_VALUE_TYPE_FLOAT) ||
      (b.type != WIL_AST_VALUE_TYPE_INTEGER &&
       b.type != WIL_AST_VALUE_TYPE_FLOAT)) {
    wil_err_emit_e(c->err, WIL_ERR_CODE_BINARY_SLASH_TYPE, expr);
    return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_NONE};
  }
  if (a.type == WIL_AST_VALUE_TYPE_INTEGER &&
      b.type == WIL_AST_VALUE_TYPE_INTEGER) {
    if (b.val.integer == 0) {
      wil_err_emit_e(c->err, WIL_ERR_CODE_DIV_ZERO, expr);
      return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_NONE};
    }
    return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_INTEGER,
                             .val.integer = a.val.integer / b.val.integer};
  }
  float lhs =
      a.type == WIL_AST_VALUE_TYPE_FLOAT ? a.val.decimal : (float)a.val.integer;
  float rhs =
      b.type == WIL_AST_VALUE_TYPE_FLOAT ? b.val.decimal : (float)b.val.integer;
  if (rhs == 0.0) {
    wil_err_emit_e(c->err, WIL_ERR_CODE_DIV_ZERO, expr);
  }
  return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_FLOAT,
                           .val.decimal = lhs / rhs};
}

CAM_STATIC wil_ast_value_t
wil_ast_eval_binary_star_(wil_ast_interpreter_context_t *c, wil_ast_value_t a,
                          wil_ast_value_t b, wil_ast_expr_t *expr) {
  if ((a.type != WIL_AST_VALUE_TYPE_INTEGER &&
       a.type != WIL_AST_VALUE_TYPE_FLOAT) ||
      (b.type != WIL_AST_VALUE_TYPE_INTEGER &&
       b.type != WIL_AST_VALUE_TYPE_FLOAT)) {
    wil_err_emit_e(c->err, WIL_ERR_CODE_BINARY_STAR_TYPE, expr);
    return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_NONE};
  }
  if (a.type == WIL_AST_VALUE_TYPE_INTEGER &&
      b.type == WIL_AST_VALUE_TYPE_INTEGER) {
    return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_INTEGER,
                             .val.integer = a.val.integer * b.val.integer};
  }
  float lhs =
      a.type == WIL_AST_VALUE_TYPE_FLOAT ? a.val.decimal : (float)a.val.integer;
  float rhs =
      b.type == WIL_AST_VALUE_TYPE_FLOAT ? b.val.decimal : (float)b.val.integer;
  return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_FLOAT,
                           .val.decimal = lhs * rhs};
}

CAM_STATIC wil_ast_value_t
wil_ast_eval_binary_minus_(wil_ast_interpreter_context_t *c, wil_ast_value_t a,
                           wil_ast_value_t b, wil_ast_expr_t *expr) {
  if ((a.type != WIL_AST_VALUE_TYPE_INTEGER &&
       a.type != WIL_AST_VALUE_TYPE_FLOAT) ||
      (b.type != WIL_AST_VALUE_TYPE_INTEGER &&
       b.type != WIL_AST_VALUE_TYPE_FLOAT)) {
    wil_err_emit_e(c->err, WIL_ERR_CODE_BINARY_MINUS_TYPE, expr);
    return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_NONE};
  }
  if (a.type == WIL_AST_VALUE_TYPE_INTEGER &&
      b.type == WIL_AST_VALUE_TYPE_INTEGER) {
    return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_INTEGER,
                             .val.integer = a.val.integer - b.val.integer};
  }
  float lhs =
      a.type == WIL_AST_VALUE_TYPE_FLOAT ? a.val.decimal : (float)a.val.integer;
  float rhs =
      b.type == WIL_AST_VALUE_TYPE_FLOAT ? b.val.decimal : (float)b.val.integer;
  return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_FLOAT,
                           .val.decimal = lhs - rhs};
}

CAM_STATIC wil_ast_value_t
wil_ast_eval_binary_plus_(wil_ast_interpreter_context_t *c, wil_ast_value_t a,
                          wil_ast_value_t b, wil_ast_expr_t *expr) {
  if ((a.type != WIL_AST_VALUE_TYPE_INTEGER &&
       a.type != WIL_AST_VALUE_TYPE_FLOAT) ||
      (b.type != WIL_AST_VALUE_TYPE_INTEGER &&
       b.type != WIL_AST_VALUE_TYPE_FLOAT)) {
    wil_err_emit_e(c->err, WIL_ERR_CODE_BINARY_PLUS_TYPE, expr);
    return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_NONE};
  }
  if (a.type == WIL_AST_VALUE_TYPE_INTEGER &&
      b.type == WIL_AST_VALUE_TYPE_INTEGER) {
    return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_INTEGER,
                             .val.integer = a.val.integer + b.val.integer};
  }
  float lhs =
      a.type == WIL_AST_VALUE_TYPE_FLOAT ? a.val.decimal : (float)a.val.integer;
  float rhs =
      b.type == WIL_AST_VALUE_TYPE_FLOAT ? b.val.decimal : (float)b.val.integer;
  return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_FLOAT,
                           .val.decimal = lhs + rhs};
}

CAM_STATIC wil_ast_value_t wil_ast_eval_binary_greater_(
    wil_ast_interpreter_context_t *c, wil_ast_value_t a, wil_ast_value_t b,
    wil_ast_expr_t *expr) {
  if (a.type == WIL_AST_VALUE_TYPE_INTEGER &&
      b.type == WIL_AST_VALUE_TYPE_INTEGER)
    return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_BOOLEAN,
                             .val.boolean = a.val.integer > b.val.integer};
  if (a.type == WIL_AST_VALUE_TYPE_FLOAT && b.type == WIL_AST_VALUE_TYPE_FLOAT)
    return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_BOOLEAN,
                             .val.boolean = a.val.decimal > b.val.decimal};
  if (a.type == WIL_AST_VALUE_TYPE_INTEGER &&
      b.type == WIL_AST_VALUE_TYPE_FLOAT)
    return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_BOOLEAN,
                             .val.boolean = a.val.integer > b.val.decimal};
  if (a.type == WIL_AST_VALUE_TYPE_FLOAT &&
      b.type == WIL_AST_VALUE_TYPE_INTEGER)
    return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_BOOLEAN,
                             .val.boolean = a.val.decimal > b.val.integer};
  wil_err_emit_e(c->err, WIL_ERR_CODE_BINARY_GREATER_TYPE, expr);
  return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_NONE};
}

CAM_STATIC wil_ast_value_t wil_ast_eval_binary_greater_equal_(
    wil_ast_interpreter_context_t *c, wil_ast_value_t a, wil_ast_value_t b,
    wil_ast_expr_t *expr) {
  if (a.type == WIL_AST_VALUE_TYPE_INTEGER &&
      b.type == WIL_AST_VALUE_TYPE_INTEGER)
    return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_BOOLEAN,
                             .val.boolean = a.val.integer >= b.val.integer};
  if (a.type == WIL_AST_VALUE_TYPE_FLOAT && b.type == WIL_AST_VALUE_TYPE_FLOAT)
    return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_BOOLEAN,
                             .val.boolean = a.val.decimal >= b.val.decimal};
  if (a.type == WIL_AST_VALUE_TYPE_INTEGER &&
      b.type == WIL_AST_VALUE_TYPE_FLOAT)
    return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_BOOLEAN,
                             .val.boolean = a.val.integer >= b.val.decimal};
  if (a.type == WIL_AST_VALUE_TYPE_FLOAT &&
      b.type == WIL_AST_VALUE_TYPE_INTEGER)
    return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_BOOLEAN,
                             .val.boolean = a.val.decimal >= b.val.integer};
  wil_err_emit_e(c->err, WIL_ERR_CODE_BINARY_GREATER_EQUAL_TYPE, expr);
  return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_NONE};
}

CAM_STATIC wil_ast_value_t
wil_ast_eval_binary_less_(wil_ast_interpreter_context_t *c, wil_ast_value_t a,
                          wil_ast_value_t b, wil_ast_expr_t *expr) {
  if (a.type == WIL_AST_VALUE_TYPE_INTEGER &&
      b.type == WIL_AST_VALUE_TYPE_INTEGER)
    return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_BOOLEAN,
                             .val.boolean = a.val.integer < b.val.integer};
  if (a.type == WIL_AST_VALUE_TYPE_FLOAT && b.type == WIL_AST_VALUE_TYPE_FLOAT)
    return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_BOOLEAN,
                             .val.boolean = a.val.decimal < b.val.decimal};
  if (a.type == WIL_AST_VALUE_TYPE_INTEGER &&
      b.type == WIL_AST_VALUE_TYPE_FLOAT)
    return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_BOOLEAN,
                             .val.boolean = a.val.integer < b.val.decimal};
  if (a.type == WIL_AST_VALUE_TYPE_FLOAT &&
      b.type == WIL_AST_VALUE_TYPE_INTEGER)
    return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_BOOLEAN,
                             .val.boolean = a.val.decimal < b.val.integer};
  wil_err_emit_e(c->err, WIL_ERR_CODE_BINARY_GREATER_TYPE, expr);
  return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_NONE};
}

CAM_STATIC wil_ast_value_t wil_ast_eval_binary_less_equal_(
    wil_ast_interpreter_context_t *c, wil_ast_value_t a, wil_ast_value_t b,
    wil_ast_expr_t *expr) {
  if (a.type == WIL_AST_VALUE_TYPE_INTEGER &&
      b.type == WIL_AST_VALUE_TYPE_INTEGER)
    return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_BOOLEAN,
                             .val.boolean = a.val.integer <= b.val.integer};
  if (a.type == WIL_AST_VALUE_TYPE_FLOAT && b.type == WIL_AST_VALUE_TYPE_FLOAT)
    return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_BOOLEAN,
                             .val.boolean = a.val.decimal <= b.val.decimal};
  if (a.type == WIL_AST_VALUE_TYPE_INTEGER &&
      b.type == WIL_AST_VALUE_TYPE_FLOAT)
    return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_BOOLEAN,
                             .val.boolean = a.val.integer <= b.val.decimal};
  if (a.type == WIL_AST_VALUE_TYPE_FLOAT &&
      b.type == WIL_AST_VALUE_TYPE_INTEGER)
    return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_BOOLEAN,
                             .val.boolean = a.val.decimal <= b.val.integer};
  wil_err_emit_e(c->err, WIL_ERR_CODE_BINARY_GREATER_EQUAL_TYPE, expr);
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
                             .val.boolean = wil_ast_equal_(a, b)};
  case WIL_LEXER_TOKEN_EQUAL_EQUAL:
    return (wil_ast_value_t){.type = WIL_AST_VALUE_TYPE_BOOLEAN,
                             .val.boolean = !wil_ast_equal_(a, b)};
  }
  wil_err_emit_e(c->err, WIL_ERR_CODE_BINARY_TYPE, expr,
                 wil_lexer_token_type_name[expr->unary.op->type]);
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
