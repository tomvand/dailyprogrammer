typedef struct dict DICT;

DICT *dict_new();
void dict_free(DICT *dict);

void *lookup(const DICT *dict, const char *key);
void insert(DICT *dict, const char *key, void *data);

char *strdup(const char *str);
