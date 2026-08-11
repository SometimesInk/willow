#ifndef WILLOW__LEXER_LEXER_UTILS_H__
#define WILLOW__LEXER_LEXER_UTILS_H__

#include <camellia/camellia.h>
#include <willow/lexer/lexer.h>
#include <willow/lexer/source.h>

extern void wil_lexer_token_dump(const wil_lexer_token_t *);

extern wil_lexer_sspan_t wil_lexer_construct_sspan(const wil_lexer_context_t *);

extern cam_int_t wil_lexer_is_at_end(const wil_lexer_context_t *);

/*
 * @brief Stores the current character to returns then advances current.
 */
extern char wil_lexer_advance(wil_lexer_context_t *);

/*
 * @brief Returns the current character without advancing.
 */
extern char wil_lexer_peek(wil_lexer_context_t *);

// TODO: TEST
/**
 * @brief Returns the next character without advancing.
 */
extern char wil_lexer_peek_next(wil_lexer_context_t *);

/**
 * @brief Checks whether the current character matches the expected character,
 * if it does, it then advances.
 */
extern cam_int_t wil_lexer_match(wil_lexer_context_t *, char);

/**
 * @brief Checks whether the current character matches the expected character,
 * if it does not, it then advances.
 * Inverse behavior to \link wil_lexer_match() \endlink whilst keeping the
 * CAM_FALSE when at end.
 * @see wil_lexer_match()
 */
extern cam_int_t wil_lexer_match_i(wil_lexer_context_t *context, char expected);

extern cam_out_t wil_lexer_add_token_simple(wil_lexer_context_t *context,
                                            const wil_lexer_token_type_t type);

extern cam_out_t wil_lexer_add_token(wil_lexer_context_t *context,
                                     const wil_lexer_token_type_t type);

extern cam_out_t wil_lexer_add_token_literal(wil_lexer_context_t *context,
                                             const wil_lexer_token_type_t type,
                                             const wil_lexer_literal_t literal);

extern void wil_lexer_identifier(wil_lexer_context_t *);

extern wil_lexer_token_type_t
wil_lexer_keyword_check(cam_str_t lit, cam_cptr_t check,
                        wil_lexer_token_type_t type);

extern wil_lexer_token_type_t
wil_lexer_identifier_is_keyword(wil_lexer_context_t *);

extern void wil_lexer_lit_num(wil_lexer_context_t *);

extern void wil_lexer_lit_str(wil_lexer_context_t *);

#endif /* WILLOW__LEXER_LEXER_UTILS_H__ */
