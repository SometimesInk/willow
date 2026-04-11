#ifndef WILLOW__SYS_ENV_H__
#define WILLOW__SYS_ENV_H__

#include <willow/willow.h>

/**
 * @brief Gets an environment variable. If it does not exist, the previous error
 * is set to `WIL_ERR_CODES_NO_ENV_VAR` and the functions fails.
 */
extern wil_out_t wil_sys_env(wil_str_t *out_ptr, const wil_cptr_t name);

#endif /* WILLOW__SYS_ENV_H__ */
