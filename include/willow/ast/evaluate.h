#ifndef WIL__AST_EVALUATE_H__
#define WIL__AST_EVALUATE_H__

#include <camellia/camellia.h>
#include <willow/ast/expressions.h>
#include <willow/err/err.h>

typedef struct {
} wil_ast_runtime_context_t;

typedef struct {
  wil_err_context_t *err;
  wil_ast_runtime_context_t *runtime;
} wil_ast_interpreter_context_t;

typedef enum {
  /**
   * @brief Used for type checking for emitting errors
   */
  WIL_AST_VALUE_TYPE_NONE,
  WIL_AST_VALUE_TYPE_INTEGER,
  WIL_AST_VALUE_TYPE_DECIMAL,
  WIL_AST_VALUE_TYPE_STRING,
  WIL_AST_VALUE_TYPE_CHAR,
  WIL_AST_VALUE_TYPE_BOOLEAN,
  WIL_AST_VALUE_TYPE_NULL,
} wil_ast_value_type_t;
#define WIL_AST_VALUE_TYPE_LEN 7

static const char *const wil_ast_value_type_name[WIL_AST_VALUE_TYPE_LEN] = {
    [WIL_AST_VALUE_TYPE_NONE] = "none",
    [WIL_AST_VALUE_TYPE_INTEGER] = "integer",
    [WIL_AST_VALUE_TYPE_DECIMAL] = "decimal",
    [WIL_AST_VALUE_TYPE_STRING] = "string",
    [WIL_AST_VALUE_TYPE_CHAR] = "char",
    [WIL_AST_VALUE_TYPE_BOOLEAN] = "boolean",
    [WIL_AST_VALUE_TYPE_NULL] = "null"};

typedef struct {
  wil_ast_value_type_t type;
  union {
    cam_int_t i;
    float d;
    cam_str_t s;
    char c;
    /**
     * @brief Runtime value should ALWAYS be either CAM_TRUE or CAM_FALSE.
     */
    cam_int_t b;
  } val;
} wil_ast_value_t;

extern wil_ast_value_t wil_ast_eval_expr(wil_ast_interpreter_context_t *c,
                                         wil_ast_expr_t *expr);

extern void wil_ast_print_eval_value(wil_ast_value_t val);

#endif /* WIL__AST_EVALUATE_H__ */
