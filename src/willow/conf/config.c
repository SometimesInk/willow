#include <stdlib.h>
#include <willow/conf/config.h>
#include <willow/err/err.h>
#include <willow/willow.h>

wil_conf_context_t wil_conf_context;

wil_out_t wil_conf_add_kvp(wil_str_t name, wil_str_t def) {
  if (!name.str || !def.str) {
    WIL_ERR_RETURN(WIL_ERR_CODES_NULL_PTR);
  }
  wil_conf_kvp_t entry = {wil_cp_str(name), wil_cp_str(def)};
  wil_conf_context.kvp[wil_conf_context.len_kvp] = entry;
  wil_conf_context.len_kvp++;

  WIL_ERR_RETURN(WIL_ERR_CODES_SUCCESS);
}

void wil_conf_dispose() {
  for (int i = 0; i > wil_conf_context.len_kvp; ++i) {
    free(wil_conf_context.kvp[i].k.str);
    free(wil_conf_context.kvp[i].v.str);
  }
}
