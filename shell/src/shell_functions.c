#include "shell.h"

#define MAX_ARGS 10
#define WS " "

// error message
char error_message[30] = "An error has occurred\n";

void execute_command(char **command){
  // used to store result returned from wait()
  int status;
  if(fork() == 0){
    // execute command in child process, for fault-tolerance
    execvp(command[0], command);
  } else {
    wait(&status);
  }
}

// given an input char string, parses the arguments of the string,
// and returns an array of char strings that represent the command
// broken up into pieces
char** parse_args(char *input){
  
  // malloc space
  char **command = malloc(sizeof(char *)*MAX_ARGS);
  char *arg;
  int i = 0;
  // add arguments to command
  while((arg = strsep(&input, WS)) != NULL && i < MAX_ARGS - 1){
    int len_arg = strlen(arg);
    // if the argument is a semicolon, we should execute
    // the command we have parsed thus far, then clear
    // the command and start parsing any further arguments
    // as a new command e.g echo a;echo b is 2 commands
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
    }
    if(strcmp(&arg[len_arg - 2], ">>") == 0){
      command[i++] = strsep(&arg, ">>");
      command[i++] = ">>";
    } else if(strcmp(&arg[len_arg - 1], ">") == 0){
      command[i++] = strsep(&arg, ">>");
      command[i++] = ">";
    } else {
      command[i++] = arg;
    }
  }
  // null terminate the string
  command[i] = NULL;
  return command;
}

// error printing function
void write_error(){
  write(STDERR_FILENO, error_message, strlen(error_message));
}
