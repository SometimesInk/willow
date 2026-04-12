#ifndef WILLOW__WILLOW_H__
#define WILLOW__WILLOW_H__

#include <stddef.h>
#include <string.h>

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

extern wil_str_t wil_create_str(const wil_cptr_t cptr);

extern wil_str_t wil_cp_str(const wil_str_t str);

#endif /* WILLOW__WILLOW_H__ */
