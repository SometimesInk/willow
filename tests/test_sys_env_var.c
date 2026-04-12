#include <willow/err/err.h>
#include <willow/sys/env.h>
#include <willow/test/test.h>
#include <willow/willow.h>

wil_test_result_t main(void) {
  wil_test_start();

  // Exist
  wil_cptr_t name1 = "PATH";
  wil_str_t env1;
  WIL_TEST_ASSERT(wil_sys_env(&env1, name1) == WIL_SUCC);
  WIL_TEST_ASSERT(wil_err_get() == WIL_ERR_CODES_SUCCESS);

  // Not exist
  wil_cptr_t name2 = "ENV_VAR_WHICH_DOES_NOT_EXIST_HOPEFULLY";
  wil_str_t env2;
  WIL_TEST_ASSERT(wil_sys_env(&env2, name2) == WIL_FAIL);
  WIL_TEST_ASSERT(wil_err_get() == WIL_ERR_CODES_NO_ENV_VAR);

  WIL_TEST_STOP_SUCC();
}
