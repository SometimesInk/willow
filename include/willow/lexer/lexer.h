#ifndef WILLOW__LEXER_LEXER_H__
#define WILLOW__LEXER_LEXER_H__

#include <camellia/camellia.h>
#include <camellia/type/dynamic_array.h>
#include <willow/err/err.h>

typedef struct {
  // TODO: wil_err_context_t *diagnostics;
  /**
   * @brief Whether an error has occurred.
   */
  cam_int_t err_count;
  // TODO: Cache length of source
  cam_cptr_t source;
  cam_type_dyn_arr_t tokens;

  // Position
  cam_int_t line;
  /**
   * @brief Start of next token.
   */
  cam_cptr_t start;
  /**
   * @brief Current cursor position.
   */
  cam_cptr_t current;
} wil_lexer_context_t;

extern wil_lexer_context_t wil_lexer_create_context(cam_cptr_t source);

extern void wil_lexer_dispose_context(wil_lexer_context_t *context);

extern cam_out_t wil_lexer_scan_tokens(wil_lexer_context_t *context,
                                       cam_type_dyn_arr_t *arr);

#endif /* WILLOW__LEXER_LEXER_H__ */
