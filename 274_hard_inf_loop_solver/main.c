#include "print.h"

#include <locale.h>

int main(int argc, char **argv) {
  grid_t grid;

  setlocale(LC_ALL, "");

  if (infloop_read(stdin, &grid) == NULL) {
    fwprintf(stderr, L"Error reading grid.\n");
    return 1;
  }

  wprintf(L"Input grid:\n");
  print_grid(stdout, &grid);

  if (infloop_solve(&grid)) {
    wprintf(L"Solution:\n");
    print_grid(stdout, &grid);
    wprintf(L"Solution data:\n");
    print_grid_data(stdout, &grid);
  } else {
    fwprintf(stderr, L"Error, unsolvable.\n");
    return 2;
  }

  infloop_free(&grid);
}
