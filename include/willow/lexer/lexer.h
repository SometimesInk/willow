#ifndef WILLOW__LEXER_LEXER_H__
#define WILLOW__LEXER_LEXER_H__

#include <camellia/camellia.h>
#include <camellia/type/dynamic_array.h>

typedef struct {
  /**
   * @brief Whether an error has occurred.
   */
  cam_int_t err_count;
  cam_cptr_t source;
  cam_type_dyn_arr_t *tokens;

  // Position
  cam_int_t line;
  cam_cptr_t start;
  cam_cptr_t current;
} wil_lexer_context_t;

extern wil_lexer_context_t wil_lexer_create_context(cam_cptr_t source);

extern cam_out_t wil_lexer_scan_tokens(wil_lexer_context_t *context,
                                       cam_type_dyn_arr_t *arr);

#endif /* WILLOW__LEXER_LEXER_H__ */
