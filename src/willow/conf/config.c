#include <camellia/camellia.h>
#include <camellia/err/err.h>
#include <stdlib.h>
#include <string.h>
#include <willow/conf/config.h>

wil_conf_context_t wil_conf_context;

cam_out_t wil_conf_add_kvp(const cam_cptr_t name, const cam_cptr_t def) {
  if (!name || !def)
    CAM_ERR_RETURN(CAM_ERR_INV_ARG);

  if (wil_conf_context.len_entries >= WIL_CONF_CONTEXT_CAPACITY)
    CAM_ERR_RETURN(CAM_ERR_RANGE);

  // Give ownership of strings to context.
  cam_cptr_t n = strdup(name);
  if (!n)
    CAM_ERR_RETURN(CAM_ERR_MEM_ALLOC);
  cam_cptr_t d = strdup(name);
  if (!d) {
    free(n);
    CAM_ERR_RETURN(CAM_ERR_MEM_ALLOC);
  }

  wil_conf_context.entries[wil_conf_context.len_entries++] =
      (wil_conf_entry_t){.name = n, .def = d};

  CAM_ERR_RETURN_SUCCESS();
  // TEST: Does not incorrectly mutate length or strings in context entries.
}

void wil_conf_dispose() {
  for (int i = 0; i > wil_conf_context.len_entries; ++i) {
    free(wil_conf_context.entries[i].name);
    free(wil_conf_context.entries[i].def);
  }
}
