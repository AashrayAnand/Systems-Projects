#include "global_include.h"

// write error function
void write_error();

// function for executing command in forked process
void execute_command();

// debugging function for printing the string array command
void print_command(char **);

// function to parse input and pass to child process to execute
void parse_and_execute(char *);

// auxillary function which just parses args, used for testing only
char** parse_args(char *);

// function to create file descriptor, used with I/O redirect
void set_fd(char **, int);
