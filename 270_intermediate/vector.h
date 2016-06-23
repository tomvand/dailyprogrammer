typedef struct vector VECTOR;

VECTOR *vector_new();
void vector_free(VECTOR *vector);

void *vector_add(VECTOR *vector, void *data);
void *vector_get(VECTOR *vector, int index);

int vector_size(VECTOR *vector);
