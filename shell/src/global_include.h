#ifndef TLPI_HDR_H
#define TLPI_HDR_H /* Prevent accidental double inclusion */
#include <sys/types.h> /* Type definitions used by many programs */
#include <stdio.h> /* Standard I/O functions */
#include <stdlib.h> /* Prototypes of commonly used library functions,
 plus EXIT_SUCCESS and EXIT_FAILURE constants */
#include <errno.h>
#include <ctype.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include <string.h> /* Commonly used string-handling functions */

#include <readline/readline.h>
#include <readline/history.h>

typedef enum { FALSE, TRUE } Boolean;
#define min(m,n) ((m) < (n) ? (m) : (n))
#define max(m,n) ((m) > (n) ? (m) : (n))
#endif
