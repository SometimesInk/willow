#include <assert.h>
#include <willow/test/test.h>

wil_test_state_t wil_test_state = WIL_TEST_STATE_NOT_TESTING;

void wil_test_start() { wil_test_state = WIL_TEST_STATE_TESTING; }

void wil_test_stop() { wil_test_state = WIL_TEST_STATE_NOT_TESTING; }
