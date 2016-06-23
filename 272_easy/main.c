// Challenge #272 [Easy]
// Tom van Dijk

#include <assert.h>
#include <stdio.h>
#include <string.h>

// Character distribution ('bag') structure
struct char_distribution {
  int char_count[27];
  int char_value[27];
  int max_count;
};

void char_distribution_init(struct char_distribution *dist) {
  for (int i = 0; i < 27; i++) {
    dist->char_count[i] = 0;
    dist->char_value[i] = 0;
  }
  dist->max_count = 0;
}

// Character from/to index conversion
int is_valid_character(char character) {
  return (character >= 'A' && character <= 'Z') || character == '_';
}

char index_to_char(int index) {
  return (char)(index == 26 ? '_' : 'A' + index);
}

int char_to_index(char character) {
  assert(is_valid_character(character));
  return (character == '_') ? 26 : character - 'A';
}

// Distribution operations
void read_distribution(FILE *file, struct char_distribution *dist) {
  assert(file != NULL);
  assert(dist != NULL);

  while (!feof(file)) {
    char buffer[128];
    fgets(buffer, sizeof(buffer), file);
    if (buffer[0] == '#') continue; // Comment, ignore

    char character;
    int count;
    int value;
    sscanf(buffer, "%c %i %i", &character, &count, &value);

    if (is_valid_character(character)) {
      int index               = char_to_index(character);
      dist->char_count[index] = count;
      dist->char_value[index] = value;
      if (count > dist->max_count) {
        dist->max_count = count;
      }
    } else {
      fprintf(stderr, "Invalid character: '%c'\n", character);
    }
  }
}

void take_from_distribution(struct char_distribution *dist, char *characters) {
  for (int i = 0; characters[i] != '\0'; i++) {
    if (!is_valid_character(characters[i])) {
      fprintf(stderr, "Invalid character: '%c'\n", characters[i]);
      continue;
    }
    int index = char_to_index(characters[i]);
    if (dist->char_count[index] > 0) {
      dist->char_count[index]--;
    } else {
      fprintf(stderr, "Invalid input. More %c's have been taken from the bag "
                      "than possible.\n",
          characters[i]);
    }
  }
}

void print_distribution(FILE *file, const struct char_distribution *dist) {
  for (int count = dist->max_count; count >= 0; count--) {
    // Loop through possible letter counts. Slow, but easy.
    _Bool line_started = 0;
    for (int character = 0; character < 27; character++) {
      if (dist->char_count[character] == count) {
        if (!line_started) {
          fprintf(file, "%i: %c", count, index_to_char(character));
          line_started = 1;
        } else {
          fprintf(file, ", %c", index_to_char(character));
        }
      }
    }
    if (line_started) fprintf(file, "\n");
  }
}


// Main entry point
int main(int argc, char **argv) {
#ifdef DEBUG
  fprintf(stderr, "%s was compiled with DEBUG flag set.\n\n", argv[0]);
#endif

  if (argc == 1) {
    printf("Usage: %s DIST [INPUT]\n", argv[0]);
    printf("DIST: letter distribution file\n");
    printf("INPUT: input data (stdin if not specified)\n");
    return 0;
  }

  struct char_distribution dist;
  char_distribution_init(&dist);
  FILE *dist_file = fopen(argv[1], "r");
  if (dist_file == NULL) {
    fprintf(stderr, "Error: can't open '%s'!\n", argv[1]);
    return 1;
  }
  read_distribution(dist_file, &dist);
  fclose(dist_file);

#ifdef DEBUG
  for (int i = 0; i < 27; i++)
    fprintf(stderr, "%c: %ix, %i pts\n", (i == 26 ? '_' : 'A' + i),
        dist.char_count[i], dist.char_value[i]);
  fprintf(stderr, "Max character count: %i\n", dist.max_count);
  fprintf(stderr, "\n\n");
  print_distribution(stderr, &dist);
#endif

  FILE *input = stdin;
  if (argc == 3) {
    input = fopen(argv[2], "r");
    if (input == NULL) {
      fprintf(stderr, "Error: can't open '%s'!\n", argv[2]);
      return 2;
    }
  }
  char buffer[128];
  while (1) {
    if (input == stdin)
      printf("\nEnter characters to remove and press enter, or an empty line "
             "to quit:\n");
    if (!fgets(buffer, sizeof(buffer), input)) break;
    strtok(buffer, "\n"); // Remove possible newline at the end
    if (buffer[0] == '\n') break;
    struct char_distribution dist_copy = dist;
    printf("\nOutput for '%s':\n", buffer);
    take_from_distribution(&dist_copy, buffer);
    print_distribution(stdout, &dist_copy);
  }
  if (input != stdin) {
    fclose(input);
  }
}
