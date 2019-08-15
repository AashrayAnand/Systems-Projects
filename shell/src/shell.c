#include "shell.h"

int main(int argc, char *argv[]){
  int i;
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

    command = parse_args(input);

    // print command
    i = 0;
    printf("command is: ");
    while(command[i]){
      printf("%s ", command[i++]);
    }
    printf("\n");

    // execute command
    execute_command(command);
  }
}
