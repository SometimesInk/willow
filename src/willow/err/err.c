#include <willow/err/err.h>
#include <willow/willow.h>

wil_str_t wil_err_codes_to_string[WIL_ERR_LEN_CODES] = {
#define X(err) {sizeof(#err) - 1, #err},
#include <willow/err/err_codes.inc>
#undef X
};

wil_err_codes_t wil_err_prev;

void wil_err_set(wil_err_codes_t err) { wil_err_prev = err; }

wil_err_codes_t wil_err_get() { return wil_err_prev; }
