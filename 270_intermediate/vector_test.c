#include "vector.h"

#include <stdio.h>

int main(int argc, char **argv) {
  VECTOR *vector = vector_new();

  int data1 = 1;
  int data2 = 50;

  vector_add(vector, &data1);
  vector_add(vector, &data2);
  printf("Vector = [%i, %i]\n", *(int *)vector_get(vector, 0),
      *(int *)vector_get(vector, 1));
  printf("Vector size = %i\n", vector_size(vector));
  printf("Vector[2] = %p\n", vector_get(vector, 2));
}
