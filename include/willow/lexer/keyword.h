#ifndef WILLOW__LEXER_KEYWORD_H__
#define WILLOW__LEXER_KEYWORD_H__

#include <camellia/camellia.h>
#include <willow/lexer/lexer.h>
#include <willow/lexer/tokens.h>

extern wil_lexer_token_type_t
wil_lexer_keyword_check(cam_str_t lit, cam_cptr_t check,
                        wil_lexer_token_type_t type);

extern wil_lexer_token_type_t
wil_lexer_identifier_is_keyword(wil_lexer_context_t *context);

#endif /* WILLOW__LEXER_KEYWORD_H__ */
