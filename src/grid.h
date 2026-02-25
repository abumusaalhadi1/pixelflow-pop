#ifndef GRID_H
#define GRID_H

#include <stdbool.h>

#include "particle.h"

/*
 * Grid size constants used across the simulation.
 */
#define GRID_WIDTH 160
#define GRID_HEIGHT 120
#define CELL_SIZE 5

/*
 * Grid stores all particles in a fixed 2D array.
 */
typedef struct {
    int width;
    int height;
    Particle cells[GRID_HEIGHT][GRID_WIDTH];
} Grid;

/*
 * initGrid sets dimensions and clears every cell.
 */
void initGrid(Grid *grid);

/*
 * resetGrid clears every cell to empty.
 */
void resetGrid(Grid *grid);

/*
 * inBounds checks whether a cell coordinate is valid.
 */
bool inBounds(const Grid *grid, int x, int y);

/*
 * getParticle returns the particle at a coordinate.
 * Returns an empty particle when out of bounds.
 */
Particle getParticle(const Grid *grid, int x, int y);

/*
 * setParticle writes a particle at a coordinate when valid.
 */
void setParticle(Grid *grid, int x, int y, Particle particle);

#endif
