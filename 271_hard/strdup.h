#ifndef STRDUP_H__
#define STRDUP_H__


#include <stdlib.h>
#include <string.h>

char *strdup(const char *str) {
  char *dup = (char *)malloc(strlen(str) + 1);
  if (dup != NULL) strcpy(dup, str);
  return dup;
}


#endif
