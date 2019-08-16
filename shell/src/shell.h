#include "global_include.h"

// write error function
void write_error();

// function for executing command in forked process
void execute_command();

// debuggin function for printing the string array command
void print_command(char **);

char** parse_args(char *);
