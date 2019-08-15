#ifndef MALLOC_DEF
#define MALLOC_DEF
#include "definitions.h"

struct head_t {
  size_t size;
  unsigned free;
  struct head_t *next;
};

void *mmalloc(size_t);
void *rrealloc(void *, size_t);
void *ccalloc(size_t, size_t);
void split_block(struct head_t *, size_t);
void ffree(void *);
void coalesce_nodes(struct head_t*);

struct head_t *get_meta(void *);
struct head_t *get_free_block(size_t);
struct head_t *request_block(size_t);

#endif
