#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 256

struct block {
  char text[BUFFER_SIZE];
  struct block *inner;
  struct block *next;
};

struct block *block_new() {
  struct block *b = malloc(sizeof(struct block));
  assert(b);
  b->text[0] = '\0';
  b->inner   = NULL;
  b->next    = NULL;
  return b;
}

void block_free(struct block *b) {
  if (b->inner != NULL) block_free(b->inner);
  if (b->next != NULL) block_free(b->next);
  free(b);
}


void parse(FILE *in, struct block *b) {
  char line[BUFFER_SIZE];

  if (fgets(line, sizeof(line), in)) {
    for (char *tok = strtok(line, " \n"); tok != NULL;
         tok       = strtok(NULL, " \n")) {

      if (strncmp(tok, "end.", 4) == 0) return;

      strncat(b->text, tok, BUFFER_SIZE);
      strncat(b->text, " ", BUFFER_SIZE);

      if (strncmp(tok, "do.", 3) == 0) {
        b->inner = block_new();
        parse(in, b->inner);
        b->next = block_new();
        parse(in, b->next);
        return;
      }
    }
    b->text[strlen(b->text) - 1] = '\0'; // Trim space at end-of-line

    b->next = block_new();
    parse(in, b->next);
  }
}

void indent(FILE *out, int depth) {
  const int SPACES = 2;
  for (int j = 0; j < depth * SPACES; j++) fputc(' ', out);
}

void print_basic(FILE *out, struct block *b) {
  static int depth = 0;

  indent(out, depth);
  fputs(b->text, out);
  fputc('\n', out);

  if (b->inner) {
    depth++;
    print_basic(out, b->inner);
    depth--;
    indent(out, depth);
    fputs("end.", out);
  }

  if (b->next) print_basic(out, b->next);
}


int main(int argc, char **argv) {
  struct block *block = block_new();
  parse(stdin, block);
  print_basic(stdout, block);
  block_free(block);

  return 0;
}
