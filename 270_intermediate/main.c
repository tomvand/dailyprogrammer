#include "dict.h"
#include "ftok.h"
#include "vector.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

const int BUFFER_SIZE = 256;

void parse(FILE *in, DICT *dict, const int PREFIX_LENGTH, const char *delim) {
  char buffer[PREFIX_LENGTH][BUFFER_SIZE];
  int buffer_head = 0;
  for (int i = 0; i < PREFIX_LENGTH; i++) buffer[i][0] = '\0';

  char key_buffer[BUFFER_SIZE];

  for (char *tok = ftok(in, delim); tok != NULL; tok = ftok(in, delim)) {
    key_buffer[0] = '\0';
    for (int i = 0; i < PREFIX_LENGTH; i++)
      strncat(
          key_buffer, buffer[(buffer_head + i) % PREFIX_LENGTH], BUFFER_SIZE);

    VECTOR *v = NULL;
    if ((v = lookup(dict, key_buffer)) == NULL)
      insert(dict, key_buffer, v = vector_new());
    vector_add(v, strdup(tok));

    strncpy(buffer[buffer_head], tok, BUFFER_SIZE);
    buffer_head = (buffer_head + 1) % (PREFIX_LENGTH);
  }
}

void write(
    FILE *out, const DICT *dict, const int PREFIX_LENGTH, int max_words) {
  char buffer[PREFIX_LENGTH][BUFFER_SIZE];
  int buffer_head = 0;
  for (int i = 0; i < PREFIX_LENGTH; i++) buffer[i][0] = '\0';

  char key_buffer[BUFFER_SIZE];
  key_buffer[0] = '\0';

  VECTOR *v;
  while ((v = lookup(dict, key_buffer)) != NULL && max_words-- > 0) {
    char *next = vector_get(v, rand() % vector_size(v));
    fprintf(out, "%s ", next);

    strncpy(buffer[buffer_head], next, BUFFER_SIZE);
    buffer_head = (buffer_head + 1) % (PREFIX_LENGTH);

    key_buffer[0] = '\0';
    for (int i = 0; i < PREFIX_LENGTH; i++)
      strncat(
          key_buffer, buffer[(buffer_head + i) % PREFIX_LENGTH], BUFFER_SIZE);
  }
  fputc('\n', out);
}

int main(int argc, char **argv) {
  if (argc != 4) {
    fprintf(
        stderr, "Usage: %s PREFIX DELIMITER LENGTH < [input file]\n", argv[0]);
    fprintf(stderr, "Example: %s 3 $' \\n' 100 < startrek.txt\n", argv[0]);
    return 0;
  }

  srand(time(NULL));

  int prefix  = atoi(argv[1]);
  char *delim = argv[2];
  int length  = atoi(argv[3]);

  DICT *dict = dict_new();
  parse(stdin, dict, prefix, delim);
  write(stdout, dict, prefix, length);
}
