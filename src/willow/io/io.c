#include <camellia/camellia.h>
#include <camellia/err/err.h>
#include <stdio.h>
#include <stdlib.h>
#include <willow/io/io.h>

cam_out_t wil_io_read_file(cam_str_t *content, const cam_cptr_t path) {
  wil_io_stream_t file = fopen(path, "rb");
  CAM_ERR_CHECK(file, CAM_ERR_IO);

  // Get file size to allocate memory accordingly
  if (fseek(file, 0L, SEEK_END)) {
    fclose(file);
    CAM_ERR_RETURN(CAM_ERR_IO);
  }
  cam_size_t file_size = ftell(file);
  if (file_size < 0) {
    fclose(file);
    CAM_ERR_RETURN(CAM_ERR_IO);
  }
  rewind(file);

  // Allocate and set memory for content
  cam_cptr_t buffer = (cam_cptr_t)malloc(file_size + 1);
  if (!buffer) {
    fclose(file);
    CAM_ERR_RETURN(CAM_ERR_MEM_ALLOC);
  }
  cam_size_t n = fread(buffer, 1, file_size, file);
  fclose(file);

  if (n != file_size) {
    free(buffer);
    CAM_ERR_RETURN(CAM_ERR_IO);
  }
  buffer[n] = '\0';

  content->str = buffer;
  content->len = n;

  CAM_ERR_RETURN_SUCCESS();
}
