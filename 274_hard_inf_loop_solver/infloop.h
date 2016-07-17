#ifndef INFLOOP_H__
#define INFLOOP_H__


#include <stdio.h>

#define GRID_INDEX(grid, row, col) ((row)*grid->width + (col))

typedef char pipe_t;

typedef struct grid {
  int width;
  int height;
  pipe_t *grid;
} grid_t;

// Read infinite loop grid from stream.
// in: input stream.
// grid: grid to read to.
// returns: pointer to grid->grid or NULL
char *infloop_read(FILE *in, grid_t *grid);

// Free memory allocated by grid.
// grid; grid to free.
void infloop_free(grid_t *grid);

// Check if the current grid is a valid solution.
// grid: grid to evaluate.
// returns: true if no pipes unconnected.
int infloop_evaluate(const grid_t *grid);

// Solve the infinite loop puzzle by rotating pipes.
// grid: grid to solve, contains solution on return.
// returns: true if a solution is found
int infloop_solve(grid_t *grid);

#endif
