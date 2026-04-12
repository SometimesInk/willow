#include <stdio.h>
#include <string.h>
#include <willow/conf/config.h>
#include <willow/err/err.h>
#include <willow/test/test.h>
#include <willow/willow.h>

#undef WIL_TEST_RETURN
#define WIL_TEST_RETURN(v)                                                     \
  do {                                                                         \
    wil_conf_dispose();                                                        \
    return v;                                                                  \
  } while (0);

wil_test_result_t main(void) {
  wil_test_start();

  wil_str_t name = wil_create_str("KEY");
  wil_str_t def = wil_create_str("DEF_VALUE");

  WIL_TEST_ASSERT_SUCC(wil_conf_add_kvp(name, def));
  WIL_TEST_ASSERT(wil_conf_context.len_kvp > 0);
  WIL_TEST_ASSERT_PREV_ERR_NOT(WIL_ERR_CODES_NULL_PTR);

  printf("kvp[0].k.str %s\n", wil_conf_context.kvp[0].k.str);
  printf("kvp[0].k.len %lu\n", wil_conf_context.kvp[0].k.len);
  printf("kvp[0].v.str %s\n", wil_conf_context.kvp[0].v.str);
  printf("kvp[0].v.len %lu\n", wil_conf_context.kvp[0].v.len);

  WIL_TEST_ASSERT(strcmp(wil_conf_context.kvp[0].k.str, name.str) == 0);
  WIL_TEST_ASSERT(wil_conf_context.kvp[0].k.len == name.len);
  WIL_TEST_ASSERT(strcmp(wil_conf_context.kvp[0].v.str, def.str) == 0);
  WIL_TEST_ASSERT(wil_conf_context.kvp[0].v.len == def.len);

  WIL_TEST_STOP_SUCC();
}
