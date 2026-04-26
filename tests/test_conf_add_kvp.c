#include <camellia/camellia.h>
#include <camellia/err/err.h>
#include <camellia/test/test.h>
#include <stdio.h>
#include <string.h>
#include <willow/conf/config.h>

#undef CAM_TEST_RETURN
#define CAM_TEST_RETURN(v)                                                     \
  do {                                                                         \
    wil_conf_dispose();                                                        \
    return v;                                                                  \
  } while (0);

cam_test_result_t main(void) {
  cam_test_start();

  cam_cptr_t name = "ENTRY_KEY";
  cam_cptr_t value = "ENTRY_VALUE";
  printf("name %s\n", name);
  printf("value%s\n", value);

  cam_out_t out = wil_conf_add_kvp(name, value);
  CAM_TEST_ASSERT_SUCCESS(out);
  CAM_TEST_ASSERT_PREV_ERR_NOT(CAM_ERR_INV_ARG);
  CAM_TEST_ASSERT_PREV_ERR_NOT(CAM_ERR_NULL_PTR);
  CAM_TEST_ASSERT(wil_conf_context.len_entries > 0);
  printf("context len %lu\n", wil_conf_context.len_entries);

  wil_conf_entry_t entry = wil_conf_context.entries[0];
  CAM_TEST_ASSERT_NOT_NULL(entry.name);
  CAM_TEST_ASSERT_NULL_TERMINATED(entry.name, entry.name_len + 1);
  CAM_TEST_ASSERT_NOT_NULL(entry.value);
  CAM_TEST_ASSERT_NULL_TERMINATED(entry.name, entry.value_len + 1);
  printf("entry name %s\n", entry.name);
  printf("entry value %s\n", entry.value);

  CAM_TEST_ASSERT(strcmp(entry.name, name) == 0);
  CAM_TEST_ASSERT(strcmp(entry.value, value) == 0);

  CAM_TEST_STOP_SUCCESS();
}
