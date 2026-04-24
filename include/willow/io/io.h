#ifndef WILLOW__IO_IO_H__
#define WILLOW__IO_IO_H__

#include <camellia/camellia.h>
#include <stdio.h>

typedef FILE *wil_io_stream_t;

extern cam_out_t wil_io_read_file(cam_str_t *content, const cam_cptr_t path);

#endif /* WILLOW__IO_IO_H__ */
