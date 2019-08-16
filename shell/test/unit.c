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
  CU_ASSERT(!blank_args);
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

// BASIC COMMAND TESTS (exit, cd, path)
void go_to_base_dir(void){
  char base_dir[] = "cd";
  char **args = parse_args(base_dir);
  char resulting_dir[PATH_MAX];
  char actual_dir[PATH_MAX];

  // get the HOME environment
  if (strncpy(actual_dir, getenv("HOME"), sizeof(actual_dir)) == NULL) {
       perror("getenv error");
  }

  execute_command(args);

  // get the directory we reach from executing cd
  if (strncpy(resulting_dir, getenv("HOME"), sizeof(resulting_dir)) == NULL) {
       perror("getenv error");
  }

  // check the result is accurate
  CU_ASSERT(strcmp(resulting_dir, actual_dir) == 0);
}

void up_one_dir(void){
  char original_dir[PATH_MAX];
  char resulting_dir[PATH_MAX];
  char actual_dir[PATH_MAX];
  char up_one[] = "cd  ..";
  char **args = parse_args(up_one);

  // get starting directory
  if (getcwd(original_dir, sizeof(original_dir)) == NULL) {
       perror("getcwd() error");
  }

  // go up one directory
  execute_command(args);

  // get resulting directory
  if (getcwd(resulting_dir, sizeof(resulting_dir)) == NULL) {
       perror("getcwd() error");
  }

  // manually change directory to original, then back one, and compare
  // the results
  if(chdir(original_dir) == -1){
    perror("chdir error");
  }

  if(chdir("..") == -1){
    perror("chdir error");
  }

  if (getcwd(actual_dir, sizeof(actual_dir)) == NULL) {
       perror("getcwd() error");
  }

  CU_ASSERT(strcmp(resulting_dir, actual_dir) == 0);
}

void same_dir(void){
  char resulting_dir[PATH_MAX];
  char actual_dir[PATH_MAX];
  char up_one[] = "cd  .";
  char **args = parse_args(up_one);

  if(getcwd(actual_dir, sizeof(actual_dir)) == NULL){
    perror("getcwd() error");
  }

  execute_command(args);

  if(getcwd(resulting_dir, sizeof(resulting_dir)) == NULL){
    perror("getcwd() error");
  }

  CU_ASSERT(strcmp(resulting_dir, actual_dir) == 0);
}

int main(){
  // set up test suite
  CU_initialize_registry();
  CU_pSuite basic_arg_suite = CU_add_suite("basic_arg_parsing", 0, 0);
  CU_pSuite basic_command_suite = CU_add_suite("basic_commands", 0, 0);

  CU_add_test(basic_arg_suite, "multiple arguments", multiple_args);
  CU_add_test(basic_arg_suite, "blank command", blank_cmd);
  CU_add_test(basic_arg_suite, "extra space", extra_space);
  CU_add_test(basic_arg_suite, "trailing spaces", trailing_space);

  CU_add_test(basic_command_suite, "change dir (going to HOME)", go_to_base_dir);
  CU_add_test(basic_command_suite, "change dir (going up one dir ../)", up_one_dir);
  CU_add_test(basic_command_suite, "change dir (staying in same dir)", same_dir);
  //CU_add_test(basic_command_suite, "path", trailing_space);

  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry();
}
