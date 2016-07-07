#ifndef VECTOR_H__
#define VECTOR_H__


typedef struct vector VECTOR;

VECTOR *vector_new();
void vector_free(VECTOR *vector);

void *vector_push(VECTOR *vector, void *data);
void *vector_pop(VECTOR *vector);
void vector_remove(VECTOR *vector, int index);
void *vector_get(VECTOR *vector, int index);

int vector_size(VECTOR *vector);


#endif
