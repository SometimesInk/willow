#ifndef WILLOW__TEST_TEST_H__
#define WILLOW__TEST_TEST_H__

#include <stdlib.h>
#include <willow/willow.h>

typedef wil_out_t wil_test_result_t;

#define WIL_TEST_FAIL ((wil_test_result_t)EXIT_FAILURE)
#define WIL_TEST_SUCC ((wil_test_result_t)EXIT_SUCCESS)

typedef enum {
  WIL_TEST_STATE_TESTING,
  WIL_TEST_STATE_NOT_TESTING
} wil_test_state_t;

extern wil_test_state_t wil_test_state;

extern void wil_test_start();
extern void wil_test_stop();

#define WIL_TEST_STOP_SUCC()                                                   \
  do {                                                                         \
    wil_test_stop();                                                           \
    return WIL_TEST_SUCC;                                                      \
  } while (0)

#define WIL_TEST_ASSERT(expr)                                                  \
  do {                                                                         \
    if (!(expr)) {                                                             \
      wil_test_stop();                                                         \
      return WIL_TEST_FAIL;                                                    \
    }                                                                          \
  } while (0)

#endif /* WILLOW__TEST_TEST_H__ */
