#include "dict.h"

#include <assert.h>
#include <stdio.h>

int main(int argc, char **argv) {
  DICT *dict = dict_new();
  assert(dict != NULL);

  int data1 = 1;
  int data2 = 50;

  printf("Insert 1...\n");
  insert(dict, "1", &data1);
  printf("Insert 2...\n");
  insert(dict, "2", &data2);

  printf(" Dict[1] = %i\n", *(int *)lookup(dict, "1"));
  printf(" Dict[2] = %i\n", *(int *)lookup(dict, "2"));
  printf("&Dict[3] = %p\n", lookup(dict, "3"));
}
