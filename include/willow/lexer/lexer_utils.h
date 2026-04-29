#ifndef WILLOW__LEXER_LEXER_UTILS_H__
#define WILLOW__LEXER_LEXER_UTILS_H__

#include <camellia/camellia.h>
#include <willow/lexer/lexer.h>
#include <willow/lexer/tokens.h>

// TODO: Add vargs support
extern void wil_lexer_err(wil_lexer_context_t *context, const cam_cptr_t msg);

extern cam_int_t wil_lexer_is_at_end(const wil_lexer_context_t *context);

// TODO: TEST
extern char wil_lexer_advance(wil_lexer_context_t *context);

// TODO: TEST
extern char wil_lexer_peek(wil_lexer_context_t *context);

// TODO: TEST
extern cam_int_t wil_lexer_match(wil_lexer_context_t *context, char expected);

// TODO: TEST
extern cam_out_t wil_lexer_add_token_simple(wil_lexer_context_t *context,
                                            const wil_lexer_token_type_t type);

// TODO: TEST
extern cam_out_t wil_lexer_add_token(wil_lexer_context_t *context,
                                     const wil_lexer_token_type_t type);

#endif /* WILLOW__LEXER_LEXER_UTILS_H__ */
