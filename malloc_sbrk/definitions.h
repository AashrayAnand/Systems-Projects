#ifndef DEFINITIONS
#define DEFINITIONS
#include <sys/types.h> /* Type definitions used by many programs */
#include <unistd.h>
#include <assert.h>

// prototype for memory allocating function
void *malloc(size_t);
// prototype for memory freeing function
void ffree(void *);

// used to align headers to 32 bytes, the union in C
// aligns data to size of the larger element, in this
// case, the 32-byte char array
typedef char ALIGN[16];
// union of a node structure and a 32-byte char array, will be
// used as the header, and will be aligned to 32 bytes
union header {
  struct {
    union header* next;
    size_t size;
    unsigned free;
  } node;
  ALIGN aligner;
};

typedef union header head_t;

struct header_t {
	size_t size;
	unsigned is_free;
};

void split_block(head_t *);

head_t* get_block(void*);

// wrapper function for sbrk, requests a block of
// memory of the specified size + size of Node
head_t* request(size_t);

#define HEAD_SIZE sizeof(head_t)

#endif
