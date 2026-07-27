#include <camellia/camellia.h>
#include <camellia/err/err.h>
#include <camellia/type/dynamic_array.h>
#include <stdlib.h>
#include <willow/lexer/lexer.h>
#include <willow/lexer/lexer_utils.h>
#include <willow/lexer/tokens.h>

wil_lexer_context_t wil_lexer_create_context(cam_cptr_t source) {
  wil_lexer_context_t buf = {
      .err_count = 0,
      .source = source,
      .tokens = {0},

      .line = 1,
      .start = source,
      .current = source,
  };

  return buf;
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
      free(token->literal.string.str);
  }

  // Free dynamic array
  cam_type_free_dyn_arr(&context->tokens);
}

cam_out_t wil_lexer_scan_tokens(wil_lexer_context_t *context,
                                cam_type_dyn_arr_t *arr) {
  while (!wil_lexer_is_at_end(context)) {
    context->start = context->current;
    if (wil_lexer_scan_one_token(context, arr) == CAM_FAILURE)
      return CAM_FAILURE;
  }

  // Add EOF token
  if (wil_lexer_add_token_simple(context, WIL_LEXER_TOKEN_EOF) == CAM_FAILURE)
    return CAM_FAILURE;
  CAM_ERR_RETURN_SUCCESS();
}
