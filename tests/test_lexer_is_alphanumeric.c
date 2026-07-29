#define CAM_TEST

#include <camellia/camellia.h>
#include <camellia/err/err.h>
#include <camellia/test/test.h>
#include <willow/io/io.h>
#include <willow/lexer/lexer.h>
#include <willow/lexer/lexer_utils.h>

cam_test_result_t main(void) {
  CAM_TEST_ASSERT(wil_lexer_is_alphanumeric('a'));
  CAM_TEST_ASSERT(wil_lexer_is_alphanumeric('m'));
  CAM_TEST_ASSERT(wil_lexer_is_alphanumeric('z'));
  CAM_TEST_ASSERT(wil_lexer_is_alphanumeric('A'));
  CAM_TEST_ASSERT(wil_lexer_is_alphanumeric('M'));
  CAM_TEST_ASSERT(wil_lexer_is_alphanumeric('Z'));
  CAM_TEST_ASSERT(wil_lexer_is_alphanumeric('_'));

  CAM_TEST_ASSERT_FAILURE(wil_lexer_is_alphanumeric('/'));
  CAM_TEST_ASSERT_FAILURE(wil_lexer_is_alphanumeric(':'));
  CAM_TEST_ASSERT(wil_lexer_is_alphanumeric('0'));
  CAM_TEST_ASSERT(wil_lexer_is_alphanumeric('1'));
  CAM_TEST_ASSERT(wil_lexer_is_alphanumeric('0'));
  CAM_TEST_ASSERT(wil_lexer_is_alphanumeric('1'));

  // These aren't numbers
  CAM_TEST_ASSERT_FAILURE(wil_lexer_is_alphanumeric('a' - 1));
  CAM_TEST_ASSERT_FAILURE(wil_lexer_is_alphanumeric('A' - 1));
  CAM_TEST_ASSERT_FAILURE(wil_lexer_is_alphanumeric('z' + 1));
  CAM_TEST_ASSERT_FAILURE(wil_lexer_is_alphanumeric('Z' + 1));

  CAM_TEST_STOP_SUCCESS();
}
