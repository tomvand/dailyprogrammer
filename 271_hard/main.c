#include "strdup.h"
#include "vector.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

#define DELIM " \n"

#define BUFFER_SIZE 256

int main(int argc, char **argv) {
  // Outline:
  // Read input line by line, word by word
  //  Keep track of newlines, indents to add keywords if req'd
  //    Add 'do.' after argument of 'for_X.' or 'if.'
  //    Ignore 'do.', already added by rule above
  //    Add 'label_.'' at newline, unless preceded by 'do.', 'else.' or 'end.'
  //      or inside 'if.' or 'for.'
  //    Remove previous 'label_.' at 'if.', 'for.' or 'else.'
  //  Linear format is the most explicit in termsof newlines and indents
  // Store tokens in linked list
  //
  // Iterate over list using appropriate printing function.
  VECTOR *tokens = vector_new();

  char buffer[BUFFER_SIZE];
  char *tok     = NULL;
  char *tok_dup = NULL;
  int is_do     = 0; // Set to 1 after reading 'if.' or 'for_X.'
  while (fgets(buffer, BUFFER_SIZE, stdin) != NULL) {
    for (tok = strtok(buffer, DELIM); tok != NULL; tok = strtok(NULL, DELIM)) {
      int s = vector_size(tokens);
      if (s > 0 && strcmp(vector_get(tokens, s - 1), "label_.") == 0 &&
          (strcmp(tok, "if.") == 0 || strncmp(tok, "for_", 4) == 0 ||
              strcmp(tok, "else.") == 0)) {
        vector_pop(tokens);
      }
      tok_dup = strdup(tok);
      assert(tok_dup != NULL);
      if (strcmp(tok_dup, "do.") != 0) {
        vector_push(tokens, (void *)tok_dup);
      }
      if (is_do) {
        vector_push(tokens, strdup("do."));
        is_do = 0;
      }
      if (strcmp(tok_dup, "if.") == 0 || strncmp(tok_dup, "for_", 4) == 0) {
        is_do = 1;
      }
    }
    if (strcmp(tok_dup, "do.") != 0 && strcmp(tok_dup, "else.") != 0 &&
        strcmp(tok_dup, "end.") != 0 && !is_do) {
      vector_push(tokens, strdup("label_.")); // Explicit newline
    }
  }

  for (int i = 0; i < vector_size(tokens); i++)
    fprintf(stdout, "%s ", (char *)vector_get(tokens, i));
}
