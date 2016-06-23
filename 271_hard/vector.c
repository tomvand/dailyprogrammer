#include "vector.h"

#include <stdlib.h>

static const int INITIAL_SIZE = 1;

struct vector {
  int size;
  int allocated_size;
  void **data;
};

static void *vector_init(VECTOR *v) {
  v->size           = 0;
  v->allocated_size = INITIAL_SIZE;
  v->data           = malloc(INITIAL_SIZE * sizeof(void *));
  return v->data;
}

VECTOR *vector_new() {
  struct vector *v = (VECTOR *)malloc(sizeof(struct vector));
  if (v != NULL) {
    if (vector_init(v) == NULL) {
      free(v);
      v = NULL;
    }
  }
  return v;
}

void vector_free(VECTOR *vector) {
  free(vector);
}


void *vector_push(VECTOR *v, void *data) {
  if (v->size + 1 > v->allocated_size) {
    int new_size = v->allocated_size * 2;
    v->data      = realloc(v->data, new_size * sizeof(void *));
    if (v->data == NULL) return NULL;
    v->allocated_size = new_size;
  }
  return v->data[v->size++] = data;
}

void *vector_pop(VECTOR *v) {
  void *data = NULL;
  if (v->size > 0) {
    data = vector_get(v, v->size - 1);
    v->size--;
  }
  return data;
}

void *vector_get(VECTOR *v, int index) {
  if (index < 0 || index >= v->size) return NULL;
  return v->data[index];
}


int vector_size(VECTOR *v) {
  return v->size;
}
