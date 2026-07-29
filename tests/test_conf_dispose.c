#define CAM_TEST

#include <camellia/camellia.h>
#include <camellia/err/err.h>
#include <camellia/test/test.h>
#include <stdio.h>
#include <willow/conf/config.h>

#undef CAM_TEST_RETURN
#define CAM_TEST_RETURN(v)                                                     \
  do {                                                                         \
    wil_conf_dispose();                                                        \
    return v;                                                                  \
  } while (0)

cam_test_result_t main(void) {
  cam_cptr_t name = "ENTRY_KEY";
  cam_cptr_t value = "ENTRY_VALUE";
  cam_out_t out = wil_conf_add_kvp(name, value);
  printf("context len %lu", wil_conf_context.len_entries);
  CAM_TEST_ASSERT(wil_conf_context.len_entries == 1);

  wil_conf_dispose();

  printf("context len %lu", wil_conf_context.len_entries);
  CAM_TEST_ASSERT(wil_conf_context.len_entries == 0);

  CAM_TEST_STOP_SUCCESS();
}
