#include "ftok.h"

int main(int argc, char **argv) {
  for (char *tok = ftok(stdin, " "); tok != NULL; tok = ftok(stdin, " "))
    puts(tok);
}
