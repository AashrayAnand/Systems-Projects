#include "shell.h"

int main(int argc, char *argv[]){
  // turn off buffering to stdout
  setvbuf(stdout, NULL, _IONBF, 0);

  for(;;){
    // read user input
    char *input = readline("ash-1.0$ ");

    // parse arguments, then pass them to child process,
    // to be executed
    parse_and_execute(input);


    // free input
    free(input);
  }
}
