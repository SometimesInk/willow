#include <camellia/camellia.h>
#include <camellia/type/dynamic_array.h>
#include <stdarg.h>
#include <stdio.h>
#include <willow/ast/expressions.h>
#include <willow/ast/parser_utils.h>
#include <willow/err/err.h>
#include <willow/lexer/source.h>

wil_err_context_t wil_err_create_context() {
  wil_err_context_t c;
  cam_type_create_dyn_arr(&c.diagnostics, sizeof(wil_err_diagnostic_t), 4);
  return c;
}

void wil_err_dispose_context(wil_err_context_t *c) {
  cam_type_free_dyn_arr(&c->diagnostics);
}

void wil_err_emit_s(wil_err_context_t *c, wil_err_code_t code,
                    wil_lexer_sspan_t sspan, ...) {
  va_list args;
  va_start(args, sspan);
  va_list copy;
  va_copy(copy, args);
  cam_size_t n = vsnprintf(CAM_NULL, 0, wil_err_code_msg[code], args);
  char buf[n + 1];
  vsnprintf(buf, n + 1, wil_err_code_msg[code], copy);

  wil_err_diagnostic_t diag = {.type = WIL_ERR_DIAGNOSTIC_TYPE_TOKEN,
                               .code = code,
                               .phase = wil_err_code_phase[code],
                               .severity = wil_err_code_severity[code],
                               .source.sspan = sspan,
                               .msg = buf};

  cam_type_push_dyn_arr(&c->diagnostics, &diag);

  va_end(args);
}

void wil_err_emit_t(wil_err_context_t *c, wil_err_code_t code,
                    wil_lexer_token_t *token, ...) {
  va_list args;
  va_start(args, token);
  va_list copy;
  va_copy(copy, args);
  cam_size_t n = vsnprintf(CAM_NULL, 0, wil_err_code_msg[code], args);
  char buf[n + 1];
  vsnprintf(buf, n + 1, wil_err_code_msg[code], copy);

  wil_err_diagnostic_t diag = {.type = WIL_ERR_DIAGNOSTIC_TYPE_TOKEN,
                               .code = code,
                               .phase = wil_err_code_phase[code],
                               .severity = wil_err_code_severity[code],
                               .source.token = token,
                               .msg = buf};

  cam_type_push_dyn_arr(&c->diagnostics, &diag);

  va_end(args);
}

void wil_err_emit_e(wil_err_context_t *c, wil_err_code_t code,
                    wil_ast_expr_t *expr, ...) {
  va_list args;
  va_start(args, expr);
  va_list copy;
  va_copy(copy, args);
  cam_size_t n = vsnprintf(CAM_NULL, 0, wil_err_code_msg[code], args);
  char buf[n + 1];
  vsnprintf(buf, n + 1, wil_err_code_msg[code], copy);

  wil_err_diagnostic_t diag = {.type = WIL_ERR_DIAGNOSTIC_TYPE_EXPR,
                               .code = code,
                               .phase = wil_err_code_phase[code],
                               .severity = wil_err_code_severity[code],
                               .source.expr = expr,
                               .msg = buf};

  cam_type_push_dyn_arr(&c->diagnostics, &diag);

  va_end(args);
}

void wil_err_print(wil_err_context_t *c) {
  for (cam_size_t i = 0; i < cam_type_len_dyn_arr(&c->diagnostics); ++i) {
    wil_err_diagnostic_t *d = cam_type_get_dyn_arr(&c->diagnostics, i);

    fprintf(stderr, "%s emitted %s %s:\n", wil_err_phase_name[d->code],
            wil_err_severity_name[d->severity], wil_err_code_name[d->code]);
    if (d->type == WIL_ERR_DIAGNOSTIC_TYPE_SSPAN) {
      fprintf(stderr, "%.*s\n", (int)d->source.sspan.len, d->source.sspan.str);
      wil_ast_pretty_print(d->source.expr);
    } else if (d->type == WIL_ERR_DIAGNOSTIC_TYPE_EXPR) {
      fprintf(stderr, " | pretty expression:\n");
      wil_ast_pretty_print(d->source.expr);
    } else if (d->type == WIL_ERR_DIAGNOSTIC_TYPE_TOKEN) {
      fprintf(stderr, " | token representation:\n");
      fprintf(stderr, "%.*s @ line %d\n", (int)d->source.token->lexeme.len,
              d->source.token->lexeme.str, d->source.token->line);
    }
    fputc('\n', stderr);
  }
}
