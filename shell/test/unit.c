#include "unit.h"
#include "../src/shell.h"

/* THIS FILE WILL COMPRISE THE UNIT TESTS FOR MY VERSIONS
    1.0 SHELL IMPLEMENTATION, "ASH", the Aashray Shell
*/

// test function that parses input line into array of char
// string arguments
void test_parse_args(void){
  char basic_ls[] = "ls -l -h -a";
  char *basic_ls_args[] = {"ls", "-l", "-h", "-a", NULL};
  char **basic_ls_args_result = parse_args(basic_ls);
  int i = 0;
  // check the string is parsed into the correct arguments
  while(basic_ls_args[i] && basic_ls_args_result[i]){
    CU_ASSERT(strcmp(basic_ls_args[i], basic_ls_args_result[i]) == 0);
    i++;
  }
}

int main(){
  // set up test suite
  CU_initialize_registry();
  CU_pSuite suite = CU_add_suite("arg_test", 0, 0);

  CU_add_test(suite, "basic_args", test_parse_args);

  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry();
}
