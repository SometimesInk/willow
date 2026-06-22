#ifndef WILLOW__LEXER_LEXER_UTILS_H__
#define WILLOW__LEXER_LEXER_UTILS_H__

#include <camellia/camellia.h>
#include <willow/lexer/lexer.h>
#include <willow/lexer/tokens.h>

// TODO: Add vargs support
// TODO: Use an enum for these errors
extern void wil_lexer_err(wil_lexer_context_t *context, const cam_cptr_t msg);

extern cam_int_t wil_lexer_is_at_end(const wil_lexer_context_t *context);

/*
 * @brief Stores the current character to returns then advances.
 */
extern char wil_lexer_advance(wil_lexer_context_t *context);

/*
 * @brief Returns the current character without advancing.
 */
extern char wil_lexer_peek(wil_lexer_context_t *context);

/**
 * @brief Checks whether the current character matches the expected character,
 * if it does, it then advances.
 */
extern cam_int_t wil_lexer_match(wil_lexer_context_t *context, char expected);

extern cam_out_t wil_lexer_add_token_simple(wil_lexer_context_t *context,
                                            const wil_lexer_token_type_t type);

// TODO: TEST
extern cam_out_t wil_lexer_add_token(wil_lexer_context_t *context,
                                     const wil_lexer_token_type_t type);

// TODO: TEST
extern cam_out_t wil_lexer_add_token_literal(wil_lexer_context_t *context,
                                             const wil_lexer_token_type_t type,
                                             const wil_lexer_literal_t literal);

#endif /* WILLOW__LEXER_LEXER_UTILS_H__ */
