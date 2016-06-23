#include "vector.h"

#include <stdlib.h>

static const int INITIAL_SIZE = 1;

struct vector {
  int size;
  int allocated_size;
  void **data;
};

VECTOR *vector_new() {
  struct vector *v = (VECTOR *)malloc(sizeof(struct vector));
  if (v != NULL) {
    v->size           = 0;
    v->allocated_size = INITIAL_SIZE;
    v->data           = malloc(INITIAL_SIZE * sizeof(void *));
    if (v->data == NULL) {
      free(v);
      return NULL;
    }
  }
  return v;
}

void vector_free(VECTOR *vector) {
  free(vector);
}


void *vector_add(VECTOR *v, void *data) {
  if (v->size + 1 > v->allocated_size) {
    int new_size = v->allocated_size * 2;
    v->data      = realloc(v->data, new_size * sizeof(void *));
    if (v->data == NULL) return NULL;
    v->allocated_size = new_size;
  }
  return v->data[v->size++] = data;
}

void *vector_get(VECTOR *v, int index) {
  if (index < 0 || index > v->size) return NULL;
  return v->data[index];
}


int vector_size(VECTOR *v) {
  return v->size;
}
