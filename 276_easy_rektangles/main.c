#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char get_rekt(char *word, int word_length, int x, int y) {
  if (x % (word_length - 1) != 0 && y % (word_length - 1) != 0) return ' ';
  int dist = (x + y) % ((word_length - 1) * 2);
  if (dist < word_length) {
    return word[dist];
  } else {
    return word[(word_length - 1) * 2 - dist];
  }
}


int main(int argc, char **argv) {
  int width  = 1;
  int height = 1;
  int depth  = 0;

  // Usage information
  if (argc < 3 || argc > 5) {
    fprintf(stderr, "Usage: %s WIDTH [HEIGHT [DEPTH]] WORD.\n", argv[0]);
    return 0;
  }

  // Read command line arguments
  width = atoi(argv[1]);
  if (argc >= 4) {
    height = atoi(argv[2]);
  } else {
    height = width;
  }
  if (argc >= 5) {
    depth = atoi(argv[3]);
  }

  // Allocate memory for output
  char *word      = argv[argc - 1];
  int word_length = strlen(word);

  int buffer_width  = width * (word_length - 1) + 2; // Including '\0'.
  int buffer_height = height * (word_length - 1) + 1;
  int buffer_depth  = depth * (word_length - 1);
  int buffer_size   = sizeof(char) * (buffer_width + buffer_depth) *
                    (buffer_height + buffer_depth);

  char *buffer = malloc(buffer_size);
  memset(buffer, ' ', buffer_size);
  for (int i = 0; i < (buffer_height + buffer_depth); i++)
    buffer[(buffer_width + buffer_depth) * (i + 1) - 1] = '\0';

  // Generate rektangle
  // Front
  for (int y = 0; y < buffer_height; y++) {
    for (int x = 0; x < buffer_width - 1; x++) {
      buffer[x + (buffer_width + buffer_depth) * y] =
          get_rekt(word, word_length, x, y);
    }
  }
  // Top
  for (int y = buffer_height; y < buffer_height + buffer_depth; y++) {
    for (int x = 0; x < buffer_width - 1; x++) {
      buffer[x + (y - buffer_height + 1) + (buffer_width + buffer_depth) * y] =
          get_rekt(word, word_length, x, y);
    }
  }
  // Side
  for (int x = buffer_width - 1; x < buffer_width - 1 + buffer_depth; x++) {
    for (int y = 0; y < buffer_height; y++) {
      buffer[x +
             (y + x - (buffer_width - 1) + 1) * (buffer_width + buffer_depth)] =
          get_rekt(word, word_length, x, y);
    }
  }

  // Output buffer
  for (int row = buffer_height + buffer_depth - 1; row >= 0; row--)
    puts(buffer + row * (buffer_width + buffer_depth));

  // Clean up
  free(buffer);
}
