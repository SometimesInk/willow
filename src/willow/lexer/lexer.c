#include <camellia/camellia.h>
#include <camellia/err/err.h>
#include <camellia/type/dynamic_array.h>
#include <threads.h>
#include <willow/lexer/lexer.h>
#include <willow/lexer/lexer_utils.h>
#include <willow/lexer/tokens.h>

wil_lexer_context_t wil_lexer_create_context(cam_cptr_t source) {
  wil_lexer_context_t buf = {
      .err_count = 0,
      .source = source,
      .tokens = CAM_NULL,

      .line = 1,
      .start = source,
      .current = source,
  };

  return buf;
}

cam_out_t wil_lexer_scan_tokens(wil_lexer_context_t *context,
                                cam_type_dyn_arr_t *arr) {
  // Initialize dynamic array
  cam_type_create_dyn_arr(arr, sizeof(wil_lexer_token_t), 8);

  while (!wil_lexer_is_at_end(context)) {
    context->start = context->current;
    if (wil_lexer_scan_one_token(context, arr) == CAM_FAILURE)
      goto fail;
  }

  // Add EOF token
  if (wil_lexer_add_token_simple(context, WIL_LEXER_TOKEN_EOF) == CAM_FAILURE)
    goto fail;
  CAM_ERR_RETURN_SUCCESS();
fail:
  cam_type_free_dyn_arr(arr);
  return CAM_FAILURE;
}
