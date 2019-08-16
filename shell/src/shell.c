#include "shell.h"

int main(int argc, char *argv[]){
  // array of char strings to store separated command

  char **command;
  // char string to read input to
  char *input;
  // turn off buffering to stdout
  setvbuf(stdout, NULL, _IONBF, 0);

  for(;;){
    // read user input
    input = readline("ash-1.0$ ");

    printf("%s is input\n", input);

    // parse arguments, returns array of string args
    command = parse_args(input);

    // print command
    print_command(command);

    // execute command
    if(command){
      execute_command(command);
    }
  }
}
