#define CAM_TEST

#include <camellia/camellia.h>
#include <camellia/err/err.h>
#include <camellia/test/test.h>
#include <willow/io/io.h>
#include <willow/lexer/lexer.h>
#include <willow/lexer/lexer_utils.h>

cam_test_result_t main(void) {
  CAM_TEST_ASSERT(wil_lexer_is_digit('0'));
  CAM_TEST_ASSERT(wil_lexer_is_digit('1'));
  CAM_TEST_ASSERT(wil_lexer_is_digit('2'));
  CAM_TEST_ASSERT(wil_lexer_is_digit('3'));
  CAM_TEST_ASSERT(wil_lexer_is_digit('4'));
  CAM_TEST_ASSERT(wil_lexer_is_digit('5'));
  CAM_TEST_ASSERT(wil_lexer_is_digit('6'));
  CAM_TEST_ASSERT(wil_lexer_is_digit('7'));
  CAM_TEST_ASSERT(wil_lexer_is_digit('8'));
  CAM_TEST_ASSERT(wil_lexer_is_digit('9'));

  CAM_TEST_ASSERT_FAILURE(wil_lexer_is_digit('/'));
  CAM_TEST_ASSERT_FAILURE(wil_lexer_is_digit(':'));

  CAM_TEST_ASSERT_FAILURE(wil_lexer_is_digit('q'));
  CAM_TEST_ASSERT_FAILURE(wil_lexer_is_digit('w'));
  CAM_TEST_ASSERT_FAILURE(wil_lexer_is_digit('e'));
  CAM_TEST_ASSERT_FAILURE(wil_lexer_is_digit('r'));
  CAM_TEST_ASSERT_FAILURE(wil_lexer_is_digit('t'));
  CAM_TEST_ASSERT_FAILURE(wil_lexer_is_digit('y'));
  CAM_TEST_ASSERT_FAILURE(wil_lexer_is_digit('u'));
  CAM_TEST_ASSERT_FAILURE(wil_lexer_is_digit('i'));
  CAM_TEST_ASSERT_FAILURE(wil_lexer_is_digit('o'));
  CAM_TEST_ASSERT_FAILURE(wil_lexer_is_digit('p'));

  CAM_TEST_STOP_SUCCESS();
}
