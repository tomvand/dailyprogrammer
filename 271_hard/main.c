#include "strdup.h"
#include "vector.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

#define DELIM " \n"

#define BUFFER_SIZE 256

void print_indent(int depth) {
  for (int i = 0; i < depth; i++) fputs("  ", stdout);
}

void print_basic(VECTOR *tokens) {
  int depth      = 0;
  int do_newline = 0;

  for (int i = 0; i < vector_size(tokens); i++) {
    char *tok = (char *)vector_get(tokens, i);
    if (strcmp(tok, "else.") == 0 || strcmp(tok, "end.") == 0) {
      depth--;
      do_newline = 1;
    } else if (strcmp(tok, "if.") == 0 || strncmp(tok, "for_", 4) == 0 ||
               strcmp(tok, "label_.") == 0) {
      do_newline = 1;
    }
    if (do_newline) {
      fputc('\n', stdout);
      print_indent(depth);
      do_newline = 0;
    }
    fprintf(stdout, "%s ", tok);
    if (strcmp(tok, "do.") == 0 || strcmp(tok, "else.") == 0) {
      depth++;
      do_newline = 1;
    } else if (strcmp(tok, "end.") == 0) {
      do_newline = 1;
    }
  }
}

void print_python(VECTOR *tokens) {
  int depth      = 0;
  int do_newline = 0;

  for (int i = 0; i < vector_size(tokens); i++) {
    char *tok = (char *)vector_get(tokens, i);
    if (strcmp(tok, "else.") == 0) {
      depth--;
      do_newline = 1;
    } else if (strcmp(tok, "if.") == 0 || strncmp(tok, "for_", 4) == 0 ||
               strcmp(tok, "label_.") == 0) {
      do_newline = 1;
    } else if (strcmp(tok, "end.") == 0) {
      depth--;
      do_newline = 0;
    }
    if (do_newline) {
      fputc('\n', stdout);
      print_indent(depth);
      do_newline = 0;
    }
    fprintf(stdout, "%s ", tok);
    if (strcmp(tok, "do.") == 0 || strcmp(tok, "else.") == 0) {
      depth++;
      do_newline = 1;
    } else if (strcmp(tok, "end.") == 0) {
      do_newline = 1;
    }
  }
}

void print_implied(VECTOR *tokens) {
  int depth      = 0;
  int do_newline = 0;

  for (int i = 0; i < vector_size(tokens); i++) {
    char *tok = (char *)vector_get(tokens, i);
    if (strcmp(tok, "else.") == 0) {
      depth--;
      do_newline = 1;
    } else if (strcmp(tok, "if.") == 0 || strncmp(tok, "for_", 4) == 0 ||
               strcmp(tok, "label_.") == 0) {
      do_newline = 1;
    } else if (strcmp(tok, "end.") == 0) {
      depth--;
      do_newline = 0;
    }
    if (do_newline) {
      fputc('\n', stdout);
      print_indent(depth);
      do_newline = 0;
    }

    if (strcmp(tok, "do.") != 0 && strcmp(tok, "end.") != 0) {
      fprintf(stdout, "%s ", tok);
    }

    if (strcmp(tok, "do.") == 0 || strcmp(tok, "else.") == 0) {
      depth++;
      do_newline = 1;
    } else if (strcmp(tok, "end.") == 0) {
      do_newline = 1;
    }
  }
}

void print_linear(VECTOR *tokens) {
  for (int i = 0; i < vector_size(tokens); i++)
    fprintf(stdout, "%s ", (char *)vector_get(tokens, i));
}

int main(int argc, char **argv) {
  // Outline:
  // Read input line by line, word by word
  //  Keep track of newlines, indents to add keywords if req'd
  //    Add 'do.' after argument of 'for_X.' or 'if.' to handle implicit
  //    Ignore 'do.', already added by rule above
  //    Add 'label_.'' at newline, will be fixed later if req'd
  //    TODO Add 'end.'
  //  Remove unnecessary 'label_.'s
  //    Remove when arg -2 is 'for_X' or 'if.'
  //    Remove when arg -1 is keyword
  //    Remove when arg +1 is keyword
  //    Remove at end of file
  //  Linear format is the most explicit in terms of newlines and indents
  // Store tokens in linked list
  //
  // Iterate over list using appropriate printing function.

  // Read list of tokens
  VECTOR *tokens = vector_new();
  char buffer[BUFFER_SIZE];
  char *tok     = NULL;
  char *tok_dup = NULL;
  int is_do     = 0; // Set to 1 after reading 'if.' or 'for_X.'
  while (fgets(buffer, BUFFER_SIZE, stdin) != NULL) {
    for (tok = strtok(buffer, DELIM); tok != NULL; tok = strtok(NULL, DELIM)) {
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
    vector_push(tokens, strdup("label_.")); // Explicit newline
  }

  // Remove 'label_.'s
  for (int i = vector_size(tokens) - 1; i >= 0; i--) {
    if (strcmp(vector_get(tokens, i), "label_.") == 0) {
      int is_eof = (i == vector_size(tokens) - 1);
      int is_before_kw =
          (i < vector_size(tokens) - 1 &&
              (strcmp(vector_get(tokens, i + 1), "else.") == 0 ||
                  strcmp(vector_get(tokens, i + 1), "if.") == 0 ||
                  strncmp(vector_get(tokens, i + 1), "for_", 4) == 0 ||
                  strcmp(vector_get(tokens, i + 1), "end.") == 0));
      int is_after_kw =
          (i >= 1 && (strcmp(vector_get(tokens, i - 1), "if.") == 0 ||
                         strncmp(vector_get(tokens, i - 1), "for_", 4) == 0 ||
                         strcmp(vector_get(tokens, i - 1), "else.") == 0));
      int is_2_after_for_if =
          (i >= 2 && (strcmp(vector_get(tokens, i - 2), "if.") == 0 ||
                         strncmp(vector_get(tokens, i - 2), "for_", 4) == 0));
      if (is_eof || is_before_kw || is_after_kw || is_2_after_for_if) {
        vector_remove(tokens, i);
      }
    }
  }

  puts("\nBasic:\n");
  print_basic(tokens);
  puts("\n");

  puts("\nLinear:\n");
  print_linear(tokens);
  puts("\n");

  puts("\nPython:\n");
  print_python(tokens);
  puts("\n");

  puts("\nImplied Python:\n");
  print_implied(tokens);
  puts("\n");
}
