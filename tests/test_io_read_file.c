#include <stdio.h>
#include <willow/io/io.h>

#include <camellia/camellia.h>
#include <camellia/err/err.h>
#include <camellia/test/test.h>

cam_test_result_t main(void) {
  cam_test_start();

  cam_str_t content;
  cam_cptr_t file = "/home/ink/Projects/willow/CHANGELOG.md";
  CAM_TEST_ASSERT_SUCCESS(wil_io_read_file(&content, file));
  CAM_TEST_ASSERT_PREV_ERR_NOT(CAM_ERR_IO);
  CAM_TEST_ASSERT_PREV_ERR_NOT(CAM_ERR_MEM_ALLOC);

  printf("len %lu\n", content.len);
  printf("%s\n", content.str);

  free(content.str);

  CAM_TEST_STOP_SUCCESS();
}
