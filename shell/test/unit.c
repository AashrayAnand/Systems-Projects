#include "unit.h"
#include "../src/shell.h"

/* THIS FILE WILL COMPRISE THE UNIT TESTS FOR MY VERSIONS
    1.0 SHELL IMPLEMENTATION, "ASH", the Aashray Shell
*/

// test function that parses input line into array of char
// string arguments
void multiple_args(void){
  char basic_ls[] = "ls -l -h -a";
  char *basic_ls_args[] = {"ls", "-l", "-h", "-a", NULL};
  char **basic_ls_args_result = parse_args(basic_ls);
  int i = 0;
  // check the string is parsed into the correct arguments
  while(basic_ls_args[i] && basic_ls_args_result[i]){
    CU_ASSERT(strcmp(basic_ls_args[i], basic_ls_args_result[i]) == 0);
    i++;
  }

  CU_ASSERT(!basic_ls_args_result[i] && !basic_ls_args[i]);
}

void blank_cmd(void){
  char blank_command[] = "";
  char **blank_args = parse_args(blank_command);
  CU_ASSERT(strlen(blank_args[0]) == 0);
}

void extra_space(void){
  char spaced_ls[] = "ls    -l    -h    -a";
  char *spaced_ls_args[] = {"ls", "-l", "-h", "-a", NULL};
  char **spaced_ls_args_result = parse_args(spaced_ls);
  int i = 0;
  // check the string is parsed into the correct arguments
  while(spaced_ls_args[i] && spaced_ls_args_result[i]){
    CU_ASSERT(strcmp(spaced_ls_args[i], spaced_ls_args_result[i]) == 0);
    i++;
  }

  CU_ASSERT(!spaced_ls_args[i] && !spaced_ls_args_result[i]);
}

void trailing_space(void){
  char trailing_cat[] = "cat shell.c    ";
  char *trailing_cat_args[] = {"cat", "shell.c", NULL};
  char **trailing_cat_args_results = parse_args(trailing_cat);
  int i = 0;
  // check the string is parsed into the correct arguments
  while(trailing_cat_args[i] && trailing_cat_args_results[i]){
    CU_ASSERT(strcmp(trailing_cat_args[i], trailing_cat_args_results[i]) == 0);
    i++;
  }

  CU_ASSERT(!trailing_cat_args_results[i] && !trailing_cat_args[i]);
}

int main(){
  // set up test suite
  CU_initialize_registry();
  CU_pSuite suite = CU_add_suite("arg_test", 0, 0);

  CU_add_test(suite, "multiple arguments", multiple_args);
  CU_add_test(suite, "blank command", blank_cmd);
  CU_add_test(suite, "extra space", extra_space);
  CU_add_test(suite, "trailing spaces", trailing_space);

  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry();
}
