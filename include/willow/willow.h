#ifndef WILLOW__WILLOW_H__
#define WILLOW__WILLOW_H__

#include <stddef.h>

typedef int wil_out_t;

#define WIL_SUCC ((wil_out_t)1)
#define WIL_FAIL ((wil_out_t)0)

#define WIL_NULL ((void *)0)

typedef char *wil_cptr_t;

typedef size_t wil_size_t;

typedef struct {
  wil_size_t len;
  wil_cptr_t str;
} wil_str_t;

#endif /* WILLOW__WILLOW_H__ */
