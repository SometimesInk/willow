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
} wil_conf_context;

extern wil_conf_context wil_conf_get_context();

extern wil_out_t wil_conf_add_kvp(wil_str_t name, wil_str_t def);

/**
 * @warning Unsafe macro.
 */
#define wil_conf_add_kvp_cptr(name, def)                                       \
  do {                                                                         \
    wil_conf_add_kvp({sizeof(name) - 1, name}, {sizeof(def) - 1, def})         \
  } while (0)

#endif /* WILLOW__CONF_CONFIG_H__ */
