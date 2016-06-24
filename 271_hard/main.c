#include "strdup.h"
#include "vector.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

#define DELIM " \n"

#define BUFFER_SIZE 256

// Printers for various formats
// For each token, the functions pre_print, on_print and post_print are called.
// pre_print and post_print can modify indent depth based on keywords, and
// request a newline by setting do_newline to TRUE.
// on_print is responsible for printing the token to out and adding a newline
// if req'd
struct printer {
  void (*pre_print)(const char *tok, int *do_newline, int *depth);
  void (*on_print)(const char *tok, int *do_newline, int depth, FILE *out);
  void (*post_print)(const char *tok, int *do_newline, int *depth);
};

void print(FILE *out, VECTOR *tokens, const struct printer *printer) {
  int depth      = 0;
  int do_newline = 0;

  for (int i = 0; i < vector_size(tokens); i++) {
    char *tok = (char *)vector_get(tokens, i);
    printer->pre_print(tok, &do_newline, &depth);
    printer->on_print(tok, &do_newline, depth, out);
    printer->post_print(tok, &do_newline, &depth);
  }
}

void print_indent(int depth, FILE *out) {
  for (int i = 0; i < depth; i++) fputs("  ", out);
}

// Basic format
void print_basic_pre(const char *tok, int *do_newline, int *depth) {
  if (strcmp(tok, "else.") == 0 || strcmp(tok, "end.") == 0) {
    (*depth)--;
    *do_newline = 1;
  } else if (strcmp(tok, "if.") == 0 || strncmp(tok, "for_", 4) == 0 ||
             strcmp(tok, "label_.") == 0) {
    *do_newline = 1;
  }
}

void print_basic_on(const char *tok, int *do_newline, int depth, FILE *out) {
  if (*do_newline) {
    fputc('\n', out);
    print_indent(depth, out);
    *do_newline = 0;
  }
  if (strcmp(tok, "label_.") != 0) fprintf(out, "%s ", tok);
}

void print_basic_post(const char *tok, int *do_newline, int *depth) {
  if (strcmp(tok, "do.") == 0 || strcmp(tok, "else.") == 0) {
    (*depth)++;
    *do_newline = 1;
  } else if (strcmp(tok, "end.") == 0) {
    *do_newline = 1;
  }
}

const struct printer PRINTER_BASIC = {
    print_basic_pre, print_basic_on, print_basic_post};

// Linear format
void print_linear_pre(const char *tok, int *do_newline, int *depth) {
}

void print_linear_on(const char *tok, int *do_newline, int depth, FILE *out) {
  fprintf(out, "%s ", tok);
}

void print_linear_post(const char *tok, int *do_newline, int *depth) {
}

const struct printer PRINTER_LINEAR = {
    print_linear_pre, print_linear_on, print_linear_post};

// Python format
void print_python_pre(const char *tok, int *do_newline, int *depth) {
  if (strcmp(tok, "else.") == 0) {
    (*depth)--;
    *do_newline = 1;
  } else if (strcmp(tok, "if.") == 0 || strncmp(tok, "for_", 4) == 0 ||
             strcmp(tok, "label_.") == 0) {
    *do_newline = 1;
  } else if (strcmp(tok, "end.") == 0) {
    (*depth)--;
    *do_newline = 0;
  }
}

const struct printer PRINTER_PYTHON = {
    print_python_pre, print_basic_on, print_basic_post};

// Implied Python
void print_implied_on(const char *tok, int *do_newline, int depth, FILE *out) {
  if (*do_newline) {
    fputc('\n', out);
    print_indent(depth, out);
    *do_newline = 0;
  }
  if (strcmp(tok, "label_.") != 0 && strcmp(tok, "do.") != 0 &&
      strcmp(tok, "end.") != 0)
    fprintf(out, "%s ", tok);
}

const struct printer PRINTER_IMPLIED = {
    print_python_pre, print_implied_on, print_basic_post};


// Parser
void parse(FILE *in, VECTOR *tokens) {
  // Outline:
  // Read input line by line, word by word
  //  Keep track of newlines, indents to add keywords if req'd
  //    Add 'do.' after argument of 'for_X.' or 'if.' to handle implicit
  //    Ignore 'do.', already added by rule above
  //    Add 'label_.'' at newline, will be fixed later if req'd
  //    TODO Add 'end.'
  // Remove unnecessary 'label_.'s
  //  Remove when arg -2 is 'for_X' or 'if.'
  //  Remove when arg -1 is keyword
  //  Remove when arg +1 is keyword
  //  Remove at end of file

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
}


// Main entry point
int main(int argc, char **argv) {
  VECTOR *tokens = vector_new();
  parse(stdin, tokens);

  puts("\n\nBasic:\n");
  print(stdout, tokens, &PRINTER_BASIC);
  puts("\n\nLinear:\n");
  print(stdout, tokens, &PRINTER_LINEAR);
  puts("\n\nPython:\n");
  print(stdout, tokens, &PRINTER_PYTHON);
  puts("\n\nImplied Python:\n");
  print(stdout, tokens, &PRINTER_IMPLIED);
  puts("\n");
}
