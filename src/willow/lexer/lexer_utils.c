#include <stdio.h>
#include <willow/lexer/lexer_utils.h>

void wil_lexer_err(wil_lexer_context_t *context, const cam_cptr_t msg) {
  printf("[ERR] %s:%lu %s", context->current_file, context->current_line, msg);
  context->err_count++;
}
