#ifndef WILLOW__CONF_CONFIG_H__
#define WILLOW__CONF_CONFIG_H__

#include <camellia/camellia.h>

#define WIL_CONF_CONTEXT_CAPACITY 512

typedef struct {
  cam_cptr_t name;
  /**
   * @brief Length of `name` excluding the null terminator.
   */
  cam_size_t name_len;
  cam_cptr_t def;
  /**
   * @brief Length of `def` excluding the null terminator.
   */
  cam_size_t def_len;
} wil_conf_entry_t;

typedef struct {
  wil_conf_entry_t entries[WIL_CONF_CONTEXT_CAPACITY];
  cam_size_t len_entries;
} wil_conf_context_t;

extern wil_conf_context_t wil_conf_context;

extern cam_out_t wil_conf_add_kvp(const cam_cptr_t name, const cam_cptr_t def);

extern void wil_conf_dispose();

extern cam_out_t wil_conf_init(const cam_cptr_t path);

#endif /* WILLOW__CONF_CONFIG_H__ */
