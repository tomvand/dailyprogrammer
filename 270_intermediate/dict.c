#include "dict.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 101

struct node {
  char *key;
  void *data;
  struct node *next;
};

struct dict {
  struct node *table[HASH_SIZE];
};

DICT *dict_new() {
  struct dict *dict = (struct dict *)malloc(sizeof(struct dict));
  for (int i = 0; i < HASH_SIZE; i++) dict->table[i] = NULL;
  return dict;
}

void dict_free(DICT *dict) {
  free(dict);
}

static unsigned int get_hash(const char *key) {
  unsigned int hash = 0;
  for (const char *p = key; *p != '\0'; p++) hash = hash * 31 + *p;
  return hash % HASH_SIZE;
}

char *strdup(const char *str) {
  char *dup = (char *)malloc(strlen(str) + 1);
  assert(dup != NULL);
  strcpy(dup, str);
  return dup;
}

static struct node *lookup_node(const DICT *dict, const char *key) {
  for (struct node *n = dict->table[get_hash(key)]; n != NULL; n = n->next) {
    if (strcmp(key, n->key) == 0) return n;
  }
  return NULL;
}

void *lookup(const DICT *dict, const char *key) {
  struct node *n = lookup_node(dict, key);
  if (n != NULL) return n->data;
  return NULL;
}

void insert(DICT *dict, const char *key, void *data) {
  struct node *n = NULL;
  if ((n = lookup_node(dict, key)) == NULL) {
    n = (struct node *)malloc(sizeof(struct node));
    assert(n != NULL);
    char *key_dup     = strdup(key);
    unsigned int hash = get_hash(key);
    n->next           = dict->table[hash];
    n->key            = key_dup;
    n->data           = data;
    dict->table[hash] = n;
  } else {
    n->data = data;
    return;
  }
}
