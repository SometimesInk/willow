#ifndef WILLOW__CONF_PARSER_H__
#define WILLOW__CONF_PARSER_H__

#include <camellia/camellia.h>
#include <willow/conf/config.h>

extern cam_out_t wil_conf_parse(const wil_conf_context_t context,
                                const cam_str_t content,
                                wil_conf_context_t *values);
#endif /* WILLOW__CONF_PARSER_H__ */
