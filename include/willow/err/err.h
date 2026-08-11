#ifndef WIL__ERR_ERR_H__
#define WIL__ERR_ERR_H__

#include <camellia/type/dynamic_array.h>
#include <willow/lexer/source.h>

typedef enum {
  WIL_ERR_SEVERITY_ERROR,
  WIL_ERR_SEVERITY_WARN,
  WIL_ERR_SEVERITY_INFO
} wil_err_severity_t;
#define WIL_ERR_SEVERITY_LEN 3

// No need for any fancy abstractions, this should not change much
static const char *const wil_err_severity_name[WIL_ERR_SEVERITY_LEN] = {
    [WIL_ERR_SEVERITY_ERROR] = "error",
    [WIL_ERR_SEVERITY_WARN] = "warning",
    [WIL_ERR_SEVERITY_INFO] = "info"};

typedef enum {
  WIL_ERR_PHASE_LEXER,
  WIL_ERR_PHASE_PARSER,
  WIL_ERR_PHASE_INTERPRETER,
} wil_err_phase_t;
#define WIL_ERR_PHASE_LEN 3

// Idem, no need for fancy abstractions
static const char *const wil_err_phase_name[WIL_ERR_PHASE_LEN] = {
    [WIL_ERR_PHASE_LEXER] = "lexer",
    [WIL_ERR_PHASE_PARSER] = "parser",
    [WIL_ERR_PHASE_INTERPRETER] = "interpreter"};

typedef enum {
#define X(name, phase, severity, msg) WIL_ERR_CODE_##name,
#include <willow/err/err_interpreter.inc>
#include <willow/err/err_lexer.inc>
#include <willow/err/err_parser.inc>
#undef X
  WIL_ERR_CODE_LEN
} wil_err_code_t;

static const char *const wil_err_code_name[WIL_ERR_CODE_LEN] = {
#define X(name, phase, severity, msg) [WIL_ERR_CODE_##name] = #name,
#include <willow/err/err_interpreter.inc>
#include <willow/err/err_lexer.inc>
#include <willow/err/err_parser.inc>
#undef X
};

static const wil_err_phase_t wil_err_code_phase[WIL_ERR_CODE_LEN] = {
#define X(name, phase, severity, msg)                                          \
  [WIL_ERR_CODE_##name] = WIL_ERR_PHASE_##phase,
#include <willow/err/err_interpreter.inc>
#include <willow/err/err_lexer.inc>
#include <willow/err/err_parser.inc>
#undef X
};

static const wil_err_severity_t wil_err_code_severity[WIL_ERR_CODE_LEN] = {
#define X(name, phase, severity, msg)                                          \
  [WIL_ERR_CODE_##name] = WIL_ERR_SEVERITY_##severity,
#include <willow/err/err_interpreter.inc>
#include <willow/err/err_lexer.inc>
#include <willow/err/err_parser.inc>
#undef X
};

static const char *const wil_err_code_msg[WIL_ERR_CODE_LEN] = {
#define X(name, phase, severity, msg) [WIL_ERR_CODE_##name] = msg,
#include <willow/err/err_interpreter.inc>
#include <willow/err/err_lexer.inc>
#include <willow/err/err_parser.inc>
#undef X
};

// Forward declare to prevent circular dependencies
typedef struct wil_lexer_token_s wil_lexer_token_t;
typedef struct wil_ast_expr_s wil_ast_expr_t;

typedef struct {
  enum {
    WIL_ERR_DIAGNOSTIC_TYPE_SSPAN,
    WIL_ERR_DIAGNOSTIC_TYPE_EXPR,
    WIL_ERR_DIAGNOSTIC_TYPE_TOKEN
  } type;
  wil_err_code_t code;
  wil_err_severity_t severity;
  wil_err_phase_t phase;

  union {
    wil_lexer_sspan_t sspan;
    wil_lexer_token_t *token;
    wil_ast_expr_t *expr;
  } source;

  const char *msg;
} wil_err_diagnostic_t;

typedef struct {
  cam_type_dyn_arr_t diagnostics;
} wil_err_context_t;

extern wil_err_context_t wil_err_create_context();

extern void wil_err_dispose_context(wil_err_context_t *c);

extern void wil_err_emit_s(wil_err_context_t *, wil_err_code_t,
                           wil_lexer_sspan_t, ...);

extern void wil_err_emit_t(wil_err_context_t *, wil_err_code_t,
                           wil_lexer_token_t *, ...);

extern void wil_err_emit_e(wil_err_context_t *, wil_err_code_t,
                           wil_ast_expr_t *, ...);

extern void wil_err_print(wil_err_context_t *);

#endif /* WIL__ERR_ERR_H__ */
