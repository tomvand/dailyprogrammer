#include "ftok.h"

#include <string.h>

#define BUFFER_SIZE 256

char *ftok(FILE *file, const char *delimiters) {
  static char buffer[BUFFER_SIZE];
  static char *tok = NULL;

  if (tok != NULL) {
    tok = strtok(NULL, delimiters);
  }
  while (tok == NULL) {
    if (fgets(buffer, BUFFER_SIZE, file) == NULL) return NULL;
    tok = strtok(buffer, delimiters);
  }
  return tok;
}
