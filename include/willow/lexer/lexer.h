#ifndef WILLOW__LEXER_LEXER_H__
#define WILLOW__LEXER_LEXER_H__

#include <camellia/camellia.h>
typedef struct {
  /**
   * @brief Name of the current file being parsed. This may be modified at
   * runtime through `include` directives. Similar to the `__FILE__` macro.
   */
  cam_cptr_t current_file;
  /**
   * @brief Current line number. This is restarted when entering a new file with
   * an `include` directive. Similar to the `__LINE__` macro.
   */
  cam_size_t current_line;
  /**
   * @brief Whether an error has occurred.
   */
  cam_int_t err_count;
} wil_lexer_context_t;

#endif /* WILLOW__LEXER_LEXER_H__ */
