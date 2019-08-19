#include "shell.h"


// flags, which are set if command involves I/O
// redirection, as well as if the redirection is
// an overwrite or an append
int redirect;
// file descriptor, will be used to redirect output
// if an I/O redirect is present in the command
int fd;

// error message
char error_message[30] = "An error has occurred\n";

void set_fd(char **input, int overwrite){
  char *file;
  if((file = strsep(input, WS)) == NULL){
    printf("error with strsep in set_fd\n");
    write_error();
    return;
  } else {
    if(overwrite){
      fd = open(file, O_CREAT|O_WRONLY|O_TRUNC);
    } else {
      fd = open(file, O_CREAT|O_WRONLY|O_APPEND);
    }
    if(fd == -1){
      printf("error with opening file %s\n", file);
      printf("Error opening file: %s\n", strerror( errno ));
      write_error();
      return;
    }
  }
  printf("successfully created fd for file %s\n", file);
}

void execute_command(char **command){
  // used to store status of executed command
  int status;
  // terminate shell on exit command

  if(strcmp(command[0], "exit") == 0){
    printf("goodbye!\n");
    exit(EXIT_SUCCESS);
  }

  if(strcmp(command[0], "cd") == 0){
    // change directory to provided argument
    if(command[1]){
      status = chdir(command[1]);
    // if no argument provided, change to
    // the user's HOME env
    } else {
      status = chdir(getenv("HOME"));
    }
    if(status == -1){
      write_error();
    }
    return;
  }

  if(fork() == 0){
    if(redirect){
      // create duplicate of stdout file descriptor, using
      // dup2 to assign it to fd created for out file
      dup2(fd, STDOUT_FILENO);
    }
    // execute command in child process, for fault-tolerance
    execvp(command[0], command);
  } else {
    wait(&status);
  }
}

void execute_piped_command(char **first_cmd, char **sec_cmd){
  int status;
  // pipe file descriptor, 0 is read end, 1 is write end
  int pipefd[2];

  if(strcmp(first_cmd[0], "exit") == 0){
    printf("goodbye!\n");
    exit(EXIT_SUCCESS);
  }

  // create pipe, return on failure
  if(pipe(pipefd) == -1){
    write_error();
    return;
  }

  if(fork() == 0){
    close(pipefd[0]);
    dup2(pipefd[1], STDOUT_FILENO);
    close(pipefd[1]);
    execvp(first_cmd[0], first_cmd);
  } else {
    if(fork() == 0){
      close(pipefd[1]);
      dup2(pipefd[0], STDIN_FILENO);
      close(pipefd[0]);
      execvp(sec_cmd[0], sec_cmd);
    } else {
      wait(&status);
    }
  }
}

// given an input char string, parses the arguments of the string,
// and returns an array of char strings that represent the command
// broken up into pieces
void parse_and_execute(char *input){
  // set redirect flag to 0
  redirect = 0;
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
        redirect = 1;
        set_fd(&input, 0);
      } else if(strcmp(&arg[len_arg - 1], ">") == 0){
        redirect = 1;
        printf("input before set_fd %s\n", input);
        set_fd(&input, 1);
        printf("input after set_fd %s\n", input);
      } else if(strcmp(&arg[len_arg - 1], "|") == 0){
        // null terminate first part of input
        // create pipefd to communicate and parse
        // second set of args, sending output
        // from first to second
        command[i] = NULL;
        i = 0;
        char **second_command = (char **) malloc(sizeof(char *)*MAX_ARGS);
        while((arg = strsep(&input, WS)) != NULL && i < MAX_ARGS - 1){
          second_command[i++] = arg;
        }
        second_command[i] = NULL;
        execute_piped_command(command, second_command);
        free(second_command);
        goto piped;
      } else {
        command[i++] = arg;
      }
    }

    // null terminate the string
    command[i] = NULL;
    if(command){
      execute_command(command);
    }

    piped:
    // free parsed command after execution
    free(command);
  }
}

// error printing function
void write_error(){
  write(STDERR_FILENO, error_message, strlen(error_message));
}

void print_command(char **command){
  int i = 0;
  printf("command is: ");
  while(command[i]){
    printf("%s ", command[i++]);
  }
  printf("\n");
}
