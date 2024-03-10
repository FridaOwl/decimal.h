#include "tests.h"

int main(void) {
  int failed = 0;
  int all = 0;
  int success = 0;
  Suite *list_cases[] = {test_convertation(), test_comparsion(),
                         test_arithmetic(), test_other(), NULL};

  for (int i = 0; list_cases[i] != NULL; i++) {
    SRunner *sr = srunner_create(list_cases[i]);

    srunner_set_fork_status(sr, CK_NOFORK);
    srunner_run_all(sr, CK_NORMAL);
    all += srunner_ntests_run(sr);
    failed += srunner_ntests_failed(sr);
    srunner_free(sr);
  }
  success = all - failed;
  printf("ALL: %d \n", all);
  printf("SUCCESS: %d \n", success);
  printf("FAILED: %d \n", failed);

  return failed == 0 ? 0 : 1;
}
