#define CAM_TEST

#include <camellia/camellia.h>
#include <camellia/err/err.h>
#include <camellia/test/test.h>
#include <willow/io/io.h>
#include <willow/lexer/lexer.h>
#include <willow/lexer/lexer_utils.h>

cam_test_result_t main(void) {
  CAM_TEST_ASSERT(wil_lexer_is_alpha('a'));
  CAM_TEST_ASSERT(wil_lexer_is_alpha('m'));
  CAM_TEST_ASSERT(wil_lexer_is_alpha('z'));
  CAM_TEST_ASSERT(wil_lexer_is_alpha('A'));
  CAM_TEST_ASSERT(wil_lexer_is_alpha('M'));
  CAM_TEST_ASSERT(wil_lexer_is_alpha('Z'));
  CAM_TEST_ASSERT(wil_lexer_is_alpha('_'));

  CAM_TEST_ASSERT_FAILURE(wil_lexer_is_alpha('/'));
  CAM_TEST_ASSERT_FAILURE(wil_lexer_is_alpha(':'));
  CAM_TEST_ASSERT_FAILURE(wil_lexer_is_alpha('0'));
  CAM_TEST_ASSERT_FAILURE(wil_lexer_is_alpha('1'));
  CAM_TEST_ASSERT_FAILURE(wil_lexer_is_alpha('0'));
  CAM_TEST_ASSERT_FAILURE(wil_lexer_is_alpha('1'));

  CAM_TEST_ASSERT_FAILURE(wil_lexer_is_alpha('a' - 1));
  CAM_TEST_ASSERT_FAILURE(wil_lexer_is_alpha('A' - 1));
  CAM_TEST_ASSERT_FAILURE(wil_lexer_is_alpha('z' + 1));
  CAM_TEST_ASSERT_FAILURE(wil_lexer_is_alpha('Z' + 1));

  CAM_TEST_STOP_SUCCESS();
}
