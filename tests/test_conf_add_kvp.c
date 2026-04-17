#include <camellia/err/err.h>
#include <camellia/test/test.h>
#include <stdio.h>
#include <willow/conf/config.h>

#undef CAM_TEST_RETURN
#define CAM_TEST_RETURN(v)                                                     \
  do {                                                                         \
    wil_conf_dispose();                                                        \
    return v;                                                                  \
  } while (0);

cam_test_result_t main(void) {
  cam_test_start();

  cam_cptr_t name = "KEY";
  printf("name.str %s\n", name);
  cam_cptr_t def = "DEF_VALUE";
  printf("def.str %s\n", def);

  CAM_TEST_ASSERT_SUCCESS(wil_conf_add_kvp(name, def));
  CAM_TEST_ASSERT_PREV_ERR_NOT(CAM_ERR_INV_ARG);
  CAM_TEST_ASSERT_PREV_ERR_NOT(CAM_ERR_NULL_PTR);
  CAM_TEST_ASSERT(wil_conf_context.len_entries > 0);

  // TEST: This line here is the point of failure. There is a seg fault likely
  // due to accessing a null pointer. Check for null in `wil_conf_add_kvp()` and
  // before this.
  printf("entries[0].name %s\n", wil_conf_context.entries[0].name);

  CAM_TEST_ASSERT(strcmp(wil_conf_context.entries[0].name, name) == 0);

  printf("entries[0].def %s\n", wil_conf_context.entries[0].def);
  CAM_TEST_ASSERT(strcmp(wil_conf_context.entries[0].def, def) == 0);

  CAM_TEST_STOP_SUCCESS();
}
