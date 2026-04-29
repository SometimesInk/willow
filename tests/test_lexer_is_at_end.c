#include <camellia/camellia.h>
#include <camellia/err/err.h>
#include <camellia/test/test.h>
#include <stdio.h>
#include <string.h>
#include <willow/io/io.h>
#include <willow/lexer/lexer.h>
#include <willow/lexer/lexer_utils.h>

cam_test_result_t main(void) {
  cam_test_start();

  // Create context
  cam_cptr_t source = "__WIL_VERSION = \"1.0.0\"\n";
  wil_lexer_context_t context = wil_lexer_create_context(source);
  CAM_TEST_ASSERT_NOT_NULL(context.source);
  CAM_TEST_ASSERT_NOT_NULL(context.current);
  CAM_TEST_ASSERT_NOT_NULL(context.start);

  // Set offset
  context.current += 5;
  printf("Current starts with: %i, expected to be: %i.\n", context.current[0],
         context.source[5]);
  CAM_TEST_ASSERT(context.current[0] == context.source[5]);

  // Check if is at end
  CAM_TEST_ASSERT_NOT(wil_lexer_is_at_end(&context));

  // Go to end
  context.current = context.source + strlen(context.source);
  printf("Current starts with: %i, expected to be: %i.\n", context.current[0],
         context.source[strlen(context.source)]);
  CAM_TEST_ASSERT(wil_lexer_is_at_end(&context));

  CAM_TEST_STOP_SUCCESS();
}
