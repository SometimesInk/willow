#include <stdlib.h>
#include <willow/err/err.h>
#include <willow/sys/env.h>

wil_out_t wil_sys_env(wil_str_t *out_ptr, const wil_cptr_t name) {
  wil_cptr_t env = getenv(name);

  // getenv(3) returns WIL_NULL when there is no such variable
  if (!env)
    WIL_ERR_RETURN(WIL_ERR_CODES_NO_ENV_VAR);

  out_ptr->len = sizeof(env) - 1;
  out_ptr->str = env;
  WIL_ERR_RETURN(WIL_ERR_CODES_SUCCESS);
}
