#include "malloc.h"

void test_allocation();
void test_free();
void test_realloc();

int main(int argc, char *argv[]){
  test_allocation();
  test_free();
  test_realloc();
  return 0;
}


void test_allocation(){
  printf("size of head_t is %lu\n", sizeof(struct head_t));
  void *x = mmalloc(sizeof(int) * 5);
  printf("x = %p\n", x);
  void *y = mmalloc(sizeof(int) * 5);
  printf("x + 5*sizeof(int) + sizeof(struct head_t) = %p\n", (x + 5*sizeof(int) + sizeof(struct head_t)));
  printf("y = %p\n", y);
  assert(y == (x + 5*sizeof(int) + sizeof(struct head_t)));
  struct head_t* x_ = get_meta(x);
  struct head_t* y_ = x_->next;
  printf("%zu", y_->size);
}

void test_free(){
  printf("allocating node 1\n");
  void *one = mmalloc(sizeof(int)* 10);
  printf("allocating node 2\n");
  void *two = mmalloc(sizeof(char)* 5);
  printf("allocating node 3\n");
  void *three = mmalloc(sizeof(short int)* 4);
  printf("allocating node 4\n");
  void *four = mmalloc(sizeof(long)* 8);
  printf("allocating node 5\n");
  void *five = mmalloc(sizeof(int)* 6);

  // in order, will free the nodes 3->2->5->4->1
  // 2 and 3 should form to become 1 node with size of
  // sizeof(struct head_t) + sizeof(char)*5 + sizeof(short int)*4
  // 4 and 5 should form to become 1 node with size of sizeof(struct head_t)
  // + sizeof(long)*8 + sizeof(int)*6
  // combining all nodes should end up with one node of size sizeof(struct head_t)*4
  // + sizeof(int)*10 + sizeof(char)*5 + sizeof(short int)*4 + sizeof(long)*8 + sizeof(int)*6
  ffree(three);
  ffree(two);
  printf("testing nodes two and three coalesce:");
  struct head_t* two_ = get_meta(two);
  assert(two_->size == sizeof(struct head_t) + sizeof(char)*5 + sizeof(short int)*4);
  printf(" passed\n");
  ffree(five);
  ffree(four);
  struct head_t* four_ = get_meta(four);
  printf("testing nodes four and five coalesce:");
  assert(four_->size == sizeof(struct head_t) + sizeof(long)*8 + sizeof(int)*6);
  printf(" passed\n");
  struct head_t* one_ = get_meta(one);
  ffree(one);
  printf("testing all nodes coalesce:");
  assert(one_->size == sizeof(int)*10 + sizeof(struct head_t)*4 + sizeof(char)*5 + sizeof(short int)*4 + sizeof(long)*8 + sizeof(int)*6);
  printf(" passed\n");
}

void test_realloc(){
  void *one = mmalloc(sizeof(int)*20);
  struct head_t* one_ = get_meta(one);
  printf("size of one is %zu\n", one_->size);
  one = rrealloc(one, sizeof(int)*60);
  one_ = get_meta(one);
  printf("size of one is %zu\n", one_->size);
}
