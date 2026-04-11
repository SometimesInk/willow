#ifndef WILLOW__ERR_ERR_H__
#define WILLOW__ERR_ERR_H__

#include <willow/willow.h>

typedef enum {
#define X(err) err,
#include <willow/err/err_codes.inc>
#undef X
  WIL_ERR_LEN_CODES,
} wil_err_codes_t;

extern wil_str_t wil_err_codes_to_string[WIL_ERR_LEN_CODES];

extern void wil_err_set(wil_err_codes_t err);

extern wil_err_codes_t wil_err_get();

#define WIL_ERR_RETURN(err)                                                    \
  do {                                                                         \
    wil_err_set(err);                                                          \
    if (err == WIL_ERR_CODES_SUCCESS)                                          \
      return WIL_SUCC;                                                         \
    return WIL_FAIL;                                                           \
  } while (0)

#endif /* WILLOW__ERR_ERR_H__ */
