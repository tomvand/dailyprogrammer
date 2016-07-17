#include "infloop.h"

#include <stdio.h>
#include <wchar.h>

static const wchar_t PIPE_WCHARS[] = {
    L' ',      // (Empty)
    L'\u2579', // N
    L'\u257A', // E
    L'\u2517', // NE
    L'\u257B', // S
    L'\u2503', // NS
    L'\u250F', // ES
    L'\u2523', // NES
    L'\u2578', // W
    L'\u251B', // NW
    L'\u2501', // EW
    L'\u253B', // NEW
    L'\u2513', // SW
    L'\u252B', // NSW
    L'\u2533', // ESW
    L'\u254B', // NESW
};

static wchar_t get_pipe_wchar(const pipe_t pipe) {
  return PIPE_WCHARS[(int)pipe];
}

void print_grid(FILE *out, const grid_t *grid) {
  for (int row = 0; row < grid->height; row++) {
    for (int col = 0; col < grid->width; col++) {
      putwchar(get_pipe_wchar(grid->grid[GRID_INDEX(grid, row, col)]));
    }
    putwchar(L'\n');
  }
}

void print_grid_data(FILE *out, const grid_t *grid) {
  for (int row = 0; row < grid->height; row++) {
    for (int col = 0; col < grid->width; col++) {
      fwprintf(out, L"%hhi ", grid->grid[GRID_INDEX(grid, row, col)]);
    }
    fwprintf(out, L"\n");
  }
}
