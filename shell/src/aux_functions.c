#include "shell.h"

// given an input char string, parses the arguments of the string,
// and returns an array of char strings that represent the command
// broken up into pieces
char** parse_args(char *input){
  // set redirect flag to 0
  if(input && strlen(input) > 0){
    char **command = (char **) malloc(sizeof(char *)*MAX_ARGS);
    // malloc space
    char *arg;
    int i = 0;
    memset(command, 0, sizeof(char *) * MAX_ARGS);
    // add arguments to command
    while((arg = strsep(&input, WS)) != NULL && i < MAX_ARGS - 1){
      // keep separating on whitespace while the
      // successive character is still additional whitespace
      int len_arg = strlen(arg);
      if(len_arg == 0){
        continue;
      }
      // if the argument is a semicolon, we should execute
      // the command we have parsed thus far, then clear
      // the command and start parsing any further arguments
      // as a new command e.g echo a;echo b is 2 commands
      /*
      if(strcmp(&arg[len_arg - 1], ";") == 0){
        // get command up until semicolon, if it was not
        // the only character in the parsed argument
        if(len_arg > 1){
          command[i++] = strsep(&arg, ";");
        }
        command[i] = NULL;
        // execute command parsed so far, then clear
        // the parsed command to make way for next command
        execute_command(command);
        memset(command, 0, sizeof(char *)*MAX_ARGS);
      }*/
      if(strcmp(&arg[len_arg - 2], ">>") == 0){
        // strsep below used to simulate set_fd function
        strsep(&input, WS);
      } else if(strcmp(&arg[len_arg - 1], ">") == 0){
        printf("input before set_fd %s\n", input);
        // strsep below used to simulate set_fd function
        strsep(&input, WS);
        printf("input after set_fd %s\n", input);
      } else {
        command[i++] = arg;
      }
    }

    // null terminate the string
    command[i] = NULL;
    return command;
  }

  return NULL;
}
