#include "infloop.h"

#include <stdlib.h>

#define PIPE_HAS_N(pipe) (pipe & 0x01)
#define PIPE_HAS_E(pipe) (pipe & 0x02)
#define PIPE_HAS_S(pipe) (pipe & 0x04)
#define PIPE_HAS_W(pipe) (pipe & 0x08)
#define PIPE_ROTATE(pipe) ((((pipe) << 1) | ((pipe) >> 3)) & 0x0F)

char *infloop_read(FILE *in, grid_t *grid) {
  fscanf(in, "%i %i", &(grid->width), &(grid->height));
  grid->grid = calloc(grid->width * grid->height, sizeof(pipe_t));
  if (grid->grid != NULL) {
    for (int row = 0; row < grid->height; row++) {
      for (int col = 0; col < grid->width; col++) {
        fscanf(in, "%hhi", grid->grid + GRID_INDEX(grid, row, col));
      }
    }
  }
  return grid->grid;
}

void infloop_free(grid_t *grid) {
  free(grid->grid);
}

int infloop_evaluate(const grid_t *grid) {
  for (int row = 0; row < grid->height; row++) {
    for (int col = 0; col < grid->width; col++) {
      char pipe = grid->grid[GRID_INDEX(grid, row, col)];
      if (PIPE_HAS_N(pipe)) {
        if (row == 0 || !PIPE_HAS_S(grid->grid[GRID_INDEX(grid, row - 1, col)]))
          return 0;
      }
      if (PIPE_HAS_E(pipe)) {
        if (col == grid->width - 1 ||
            !PIPE_HAS_W(grid->grid[GRID_INDEX(grid, row, col + 1)]))
          return 0;
      }
      if (PIPE_HAS_S(pipe)) {
        if (row == grid->height - 1 ||
            !PIPE_HAS_N(grid->grid[GRID_INDEX(grid, row + 1, col)]))
          return 0;
      }
      if (PIPE_HAS_W(pipe)) {
        if (col == 0 || !PIPE_HAS_E(grid->grid[GRID_INDEX(grid, row, col - 1)]))
          return 0;
      }
    }
  }
  return 1;
}

static int infloop_solve_recursive(grid_t *grid, int index) {
  if (index >= grid->width * grid->height) return 0;
  for (int i = 0; i < 4; i++) {
    grid->grid[index] = PIPE_ROTATE(grid->grid[index]);
    if (infloop_evaluate(grid)) return 1;
    if (infloop_solve_recursive(grid, index + 1)) return 1;
  }
  return 0;
}

int infloop_solve(grid_t *grid) {
  return infloop_solve_recursive(grid, 0);
}
