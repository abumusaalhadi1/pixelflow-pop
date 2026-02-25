#include "grid.h"

/*
 * initGrid sets dimensions and clears every cell.
 */
void initGrid(Grid *grid) {
    grid->width = GRID_WIDTH;
    grid->height = GRID_HEIGHT;
    resetGrid(grid);
}

/*
 * resetGrid clears every cell to empty.
 */
void resetGrid(Grid *grid) {
    int x;
    int y;

    for (y = 0; y < grid->height; y++) {
        for (x = 0; x < grid->width; x++) {
            grid->cells[y][x] = createEmptyParticle();
        }
    }
}

/*
 * inBounds checks whether a cell coordinate is valid.
 */
bool inBounds(const Grid *grid, int x, int y) {
    return (x >= 0 && x < grid->width && y >= 0 && y < grid->height);
}

/*
 * getParticle returns the particle at a coordinate.
 * Returns an empty particle when out of bounds.
 */
Particle getParticle(const Grid *grid, int x, int y) {
    if (!inBounds(grid, x, y)) {
        return createEmptyParticle();
    }

    return grid->cells[y][x];
}

/*
 * setParticle writes a particle at a coordinate when valid.
 */
void setParticle(Grid *grid, int x, int y, Particle particle) {
    if (!inBounds(grid, x, y)) {
        return;
    }

    grid->cells[y][x] = particle;
}
