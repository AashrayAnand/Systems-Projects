#include "malloc.h"
#include "definitions.h"

// global lock, must be accquired by any threads that are
// attempting to access memory
pthread_mutex_t malloc_lock;

// head of free list
struct head_t* head;

// tail of free list
struct head_t* tail;

// given a pointer to a block of memory, return the meta
// data about this memory block
struct head_t *get_meta(void *block){
  return (struct head_t*)block - 1;
}

struct head_t *request_block(size_t size){
  struct head_t *block;
  // must move program break by the specified amount
  // of bytes (size), as well as the size of the head_t
  // meta data structure
  block = sbrk(size + sizeof(struct head_t));
  // check if sbrk failed
  if((void *) block == (void *) -1){
    printf("error allocating block of memory\n");
    return NULL;
  }

  // for first request (NULL free list), set head to point to
  // newly allocated memory
  if(!head){
    head = block;
  }

  if(tail){
    tail->next = block;
  }
  // update the tail pointer
  tail = block;

  // set the meta data for the memory block
  block->size = size;
  block->free = 0;
  block->next = NULL;

  // return a pointer to the front of the memory block, instead
  // of the meta data header structure
  return block + 1;
}

struct head_t *get_free_block(size_t size){
  struct head_t *curr = head;
  while(curr && !(curr->size >= size && curr->free)){
    curr = curr->next;
  }
  return curr;
}

void split_block(struct head_t *block, size_t size){
  size_t excess = block->size - size;
}

void ffree(void *ptr){
  pthread_mutex_lock(&malloc_lock);
  // attempting to free NULL, simply return
  if(!ptr){
    pthread_mutex_unlock(&malloc_lock);
    return;
  }
  struct head_t *meta = get_meta(ptr);

  // attempting to free block that is already freed, this
  // is an error
  if(meta->free){
    fprintf(stderr, "error, attempting to free memory that has already been freed\n");
    pthread_mutex_unlock(&malloc_lock);
    return;
  }

  meta->free = 1;
  // continuouslly combine successor elements that are free
  // with the memiory block that has just been freed
  coalesce_nodes(meta);

  pthread_mutex_unlock(&malloc_lock);
}

// given a header of a node in the free list, successively combines
// it with successor nodes until we reach a node that is not free
void coalesce_nodes(struct head_t* meta){
  while(meta->next && meta->next->free){
    // absorb memory block and update to point to next block
    // in the free list
    meta->size += sizeof(struct head_t) + meta->next->size;
    meta->next = meta->next->next;
  }
}
// given a specified size (byte count), allocate a block of
// memory of the specified size, or return a pointer to a previously
// allocated block of memory, which is free and satisfies the
// prerequisite size requirement
void *mmalloc(size_t size){
  if(size <= 0){
    return NULL;
  }

  struct head_t *block;
  // accquire lock before attempting to get a memory block
  pthread_mutex_lock(&malloc_lock);
  block = get_free_block(size);
  // appropriate block for specified size was found
  if(block){
    block->free = 0;
    // use pointer arithmetic to advance block to point to
    // the beginning of the memory block (after the meta data)
    block += 1;
  } else {
    block = request_block(size);
  }

  // block has been selected/allocated, give up lock
  pthread_mutex_unlock(&malloc_lock);
  return (void *) block;
}

// given an existing dynamically allocated memory pointer, reallocate
// the pointer to a different size. two possible cases are that the
// node can be extended with any free next nodes, if not, must allocate a
// a new block of memory, copy over the information from the current block
// of memory, and free the block
void *rrealloc(void *node, size_t size){
  // when node is freeds, it coalesces with any successive free nodes,
  // until we reach a node that is not free, if the node is not large enough after
  // being freed, we will need to request more memory
  if(!node || !size){
    // will return NULL if size is 0, or request new block
    // if memory pointer is NULL
    return mmalloc(size);
  }

  void* ptr;
  struct head_t* block;
  struct head_t* meta = get_meta(node);

  // block is not big enough, allocate a new block, copy
  // data from current block, and free current block, returning
  // the newly allocated block
  if(meta->size < size){
    ptr = mmalloc(size);
    memcpy(ptr, node, meta->size);
    ffree(node);
    return ptr;
  }

  // use the existing memory block
  return node;
}

void *ccalloc(size_t count, size_t size){
  size_t total_size;
  total_size = size * count;
  // check for integer overflow
  if(count != total_size / size){
    return NULL;
  }

  void* ptr = mmalloc(total_size);
  // return NULL for failed mmalloc
  if(!ptr){
    return NULL;
  }

  // zero out the byte string
  memset(ptr, 0, count * size);
  return ptr;
}
