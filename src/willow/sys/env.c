#include <camellia/err/err.h>
#include <stdlib.h>
#include <willow/sys/env.h>

cam_out_t wil_sys_env(cam_str_t *out_ptr, const cam_cptr_t name) {
  cam_cptr_t env = getenv(name);

  // getenv(3) returns `CAM_NULL` when there is no such variable
  if (!env)
    CAM_ERR_RETURN(CAM_ERR_NO_ENV_VAR);

  out_ptr->len = sizeof(env) - 1;
  out_ptr->str = env;
  CAM_ERR_RETURN_SUCCESS();
}
