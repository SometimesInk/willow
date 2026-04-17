#include <willow/sys/env.h>

#include <camellia/camellia.h>
#include <camellia/err/err.h>
#include <camellia/test/test.h>

cam_test_result_t main(void) {
  cam_test_start();

  // Exist
  cam_cptr_t name1 = "PATH";
  cam_str_t env1;
  CAM_TEST_ASSERT_SUCCESS(wil_sys_env(&env1, name1));
  CAM_TEST_ASSERT_PREV_ERR(CAM_ERR_SUCCESS);

  // Not exist
  cam_cptr_t name2 = "ENV_VAR_WHICH_hopefully_DOES_NOT_EXIST";
  cam_str_t env2;
  CAM_TEST_ASSERT_FAILURE(wil_sys_env(&env2, name2));
  CAM_TEST_ASSERT_PREV_ERR(CAM_ERR_NO_ENV_VAR);

  CAM_TEST_STOP_SUCCESS();
}
