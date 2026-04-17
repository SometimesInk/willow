#ifndef WILLOW__SYS_ENV_H__
#define WILLOW__SYS_ENV_H__

#include <camellia/camellia.h>

/**
 * @brief Gets an environment variable. If it does not exist, previous error
 * is set to `NO_ENV_VAR` and the functions fails.
 */
extern cam_out_t wil_sys_env(cam_str_t *out_ptr, const cam_cptr_t name);

#endif /* WILLOW__SYS_ENV_H__ */
