#ifndef WIL__LEXER_SOURCE_H__
#define WIL__LEXER_SOURCE_H__

#include <camellia/camellia.h>

typedef struct {
  cam_str_t sbuf;
} wil_lexer_src_t;

/**
 * @brief Used for pointers owned by the source buffer, thus pointers that point
 * within the source buffer.
 */
typedef char *wil_lexer_sptr_t;

/**
 * @brief Span within source buffer. `str` should be a `wil_lexer_sptr_t`.
 */
typedef cam_str_t wil_lexer_sspan_t;

#endif /* WIL__LEXER_SOURCE_H__ */
