#include "definitions.h"

head_t* head = NULL;
head_t* tail = NULL;

int main(int argc, char *argv[]){
  // pointer returned is at the start of the memory block, which is 1 byte,
  // second pointer should be 1 + sizeof(Node *) higher address
  void* x = malloc(24);
  printf("address of x (block of 1 byte): %p\n", x);
  void* y = malloc(24);
  printf("address of y (block of 1 byte): %p\n", y);
  assert((x + HEAD_SIZE + 24) == y);
  printf("address of x + sizeof(Node) + 24 =  %p = address of y\n", (x + HEAD_SIZE + 24));
  printf("address of x is %p, address of x + 1 is %p\n", x, (x + 1));
  ffree(x);
  head_t* b = get_block(x);
  assert(b->node.free == 1);
  printf("size of header: %lu\n", HEAD_SIZE);
  printf("size of header_t: %lu\n", sizeof(struct header_t));
  return 0;
}

head_t* get_block(void* ptr){
  // using pointer arithmetic, cast pointer to front of the
  // block to a header pointer, and go back by the size of 1 header
  // pointer, to access the meta-info of the memory block
  return (head_t *)ptr - 1;
}

void ffree(void* ptr){
  if(ptr){
    // get meta-info for allocated memory
    head_t* block = get_block(ptr);
    // ensure the block hasn't already been free'd
    assert(block->node.free == 0);
    // "free memory", simply release it back to the free list,
    // can now be used for future malloc calls
    block->node.free = 1;
  }
}

head_t* request(size_t size){
  head_t* block;
  // shift program break by size bytes + size of
  // node struct, and store the front of the new
  // memory block
  block = sbrk(size + HEAD_SIZE);
  // change the location of the program break
  // by size bytes, plus the size of the free
  // list node structure
  if((void *) block == (void *) -1){
    return NULL;
  }

  // NULL for first request, otherwise, block
  // added to tail of list
  if(tail){
    tail->node.next = block;
  }

  // newly requested block will become the tail of the free list
  tail = block;

  // set the fields of the block
  block->node.size = size;
  block->node.free = 0;
  block->node.next = NULL;

  return block;
}

head_t* find_block(size_t size){
  head_t* curr = head;
  while(curr && !(curr->node.size >= size && curr->node.free)){
    curr = curr->node.next;
  }
  // will be NULL if no appropriate blocks found
  return curr;
}

void *malloc(size_t size){
  head_t* block;
  // malloc will be given a size, should either:
  // 0. return NULL for size <= 0
  // 1. request a block of the expected size, if the
  // head of the free list is NULL (first requesst)
  // 2. find and return an existing block of the appropriate
  // size (also, split the block at the appropriate position)
  // 3. allocate a new block of size + struct memory, if no
  // blocks are the right size
  if(size <= 0){
    return NULL;
  }

  // first malloc request, no memory allocated
  if(!head){
    block = request(size);
    if(!block){
      return NULL;
    }
    head = block;
  } else {
    block = find_block(size);
    // no block of suitable size was found, allocate
    // a new block instead
    if(!block){
      block = request(size);
      // failed to allocate new block
      if(!block){
        return NULL;
      }
    } else {
      block->node.free = 0;
    }
  }
  // by pointer arithmetic, returning block + 1 returns
  // the memory address that is sizeof(struct head_t*) from
  // block, which is the location of the memory block that
  // has been allocated
  return block + 1;
}
