#ifndef WILLOW__CONF_CONFIG_H__
#define WILLOW__CONF_CONFIG_H__

#include <willow/willow.h>

typedef struct {
  wil_str_t k;
  wil_str_t v;
} wil_conf_kvp_t;

typedef struct {
  wil_size_t len_kvp;
  wil_conf_kvp_t kvp[];
} wil_conf_context_t;

extern wil_conf_context_t wil_conf_context;

extern wil_out_t wil_conf_add_kvp(wil_str_t name, wil_str_t def);

extern void wil_conf_dispose();

#endif /* WILLOW__CONF_CONFIG_H__ */
