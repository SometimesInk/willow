#include <string.h>
#include <willow/err/err.h>
#include <willow/willow.h>

wil_str_t wil_create_str(const wil_cptr_t cptr) {
  wil_str_t str = {strlen(cptr), cptr};
  return str;
}

wil_str_t wil_cp_str(const wil_str_t str) {
  wil_str_t new = {str.len, strdup(str.str)};
  return new;
}
