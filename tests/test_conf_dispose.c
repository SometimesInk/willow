#include <camellia/camellia.h>
#include <camellia/err/err.h>
#include <camellia/test/test.h>
#include <stdio.h>
#include <willow/conf/config.h>

cam_test_result_t main(void) {
  cam_test_start();

  cam_cptr_t name = "ENTRY_KEY";
  cam_cptr_t def = "ENTRY_DEF_VALUE";
  cam_out_t out = wil_conf_add_kvp(name, def);
  printf("context len %lu", wil_conf_context.len_entries);
  CAM_TEST_ASSERT(wil_conf_context.len_entries == 1);

  wil_conf_dispose();

  printf("context len %lu", wil_conf_context.len_entries);
  CAM_TEST_ASSERT(wil_conf_context.len_entries == 0);

  CAM_TEST_STOP_SUCCESS();
}
