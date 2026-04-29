#include <camellia/camellia.h>
#include <camellia/err/err.h>
#include <stdlib.h>
#include <string.h>
#include <willow/conf/config.h>
#include <willow/conf/parser.h>
#include <willow/io/io.h>

wil_conf_context_t wil_conf_context;

cam_out_t wil_conf_add_kvp(const cam_cptr_t name, const cam_cptr_t def) {
  if (!name || !def)
    CAM_ERR_RETURN(CAM_ERR_INV_ARG);

  if (wil_conf_context.len_entries >= WIL_CONF_CONTEXT_CAPACITY)
    CAM_ERR_RETURN(CAM_ERR_RANGE);

  // Allocate memory for entry
  cam_size_t len_name = strlen(name) + 1;
  cam_cptr_t entry_name = (cam_cptr_t)malloc(len_name);
  if (!entry_name)
    CAM_ERR_RETURN(CAM_ERR_MEM_ALLOC);
  memcpy(entry_name, name, len_name);

  cam_size_t len_def = strlen(def) + 1;
  cam_cptr_t entry_def = (cam_cptr_t)malloc(len_def);
  if (!entry_def) {
    free(entry_name);
    CAM_ERR_RETURN(CAM_ERR_MEM_ALLOC);
  }
  memcpy(entry_def, def, len_def);

  wil_conf_context.entries[wil_conf_context.len_entries++] =
      (wil_conf_entry_t){.name = entry_name,
                         .name_len = len_name - 1,
                         .value = entry_def,
                         .value_len = len_def - 1};

  CAM_ERR_RETURN_SUCCESS();
}

void wil_conf_dispose() {
  for (int i = 0; i > wil_conf_context.len_entries; ++i) {
    free(wil_conf_context.entries[i].name);
    free(wil_conf_context.entries[i].value);
  }
  wil_conf_context.len_entries = 0;
}

// TODO: TEST,
cam_out_t wil_conf_init(const cam_cptr_t path) {
  CAM_ERR_CHECK(wil_conf_context.len_entries != 0, CAM_ERR_NOT_ENOUGH_ARGS);

  cam_str_t file_content;
  if (wil_io_read_file(&file_content, path) == CAM_FAILURE)
    return CAM_FAILURE;
  // if (wil_conf_parse(file_content))
  //   return CAM_FAILURE;

  CAM_ERR_RETURN_SUCCESS();
}
