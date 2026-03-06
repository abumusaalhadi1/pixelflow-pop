#include "../include/physics.h"

#include <stdbool.h>
#include <stdlib.h>

/*
 * markUpdated flags a cell as handled only when inside grid bounds.
 */
static void markUpdated(const Grid *grid, bool updated[GRID_HEIGHT][GRID_WIDTH], int x, int y) {
    if (!inBounds(grid, x, y)) {
        return;
    }

    updated[y][x] = true;
}

/*
 * tryMoveParticle moves a particle when the destination is empty.
 */
static bool tryMoveParticle(Grid *grid, int fromX, int fromY, int toX, int toY, bool updated[GRID_HEIGHT][GRID_WIDTH]) {
    if (!inBounds(grid, toX, toY) || !inBounds(grid, fromX, fromY)) {
        return false;
    }

    Particle destination = getParticle(grid, toX, toY);
    Particle source = getParticle(grid, fromX, fromY);

    if (destination.type != PARTICLE_EMPTY) {
        return false;
    }

    setParticle(grid, toX, toY, source);
    setParticle(grid, fromX, fromY, createEmptyParticle());
    markUpdated(grid, updated, toX, toY);
    return true;
}

/*
 * tryDissolveWithAcid applies acid reaction with a delay for better visuals.
 */
static bool tryDissolveWithAcid(Grid *grid, int acidX, int acidY, int targetX, int targetY, bool updated[GRID_HEIGHT][GRID_WIDTH]) {
    if (!inBounds(grid, acidX, acidY) || !inBounds(grid, targetX, targetY)) {
        return false;
    }

    Particle acid = getParticle(grid, acidX, acidY);
    Particle target = getParticle(grid, targetX, targetY);

    if (acid.type != PARTICLE_ACID) {
        return false;
    }

    if (target.type == PARTICLE_EMPTY || target.type == PARTICLE_ACID) {
        return false;
    }

    // Don't dissolve every frame. This makes contact look gradual.
    if (rand() % 100 >= 35) {
        return false;
    }

    // Brief smoke gives visual feedback that a dissolve happened.
    if (rand() % 4 == 0) {
        setParticle(grid, targetX, targetY, createParticle(PARTICLE_SMOKE));
    } else {
        setParticle(grid, targetX, targetY, createEmptyParticle());
    }
    markUpdated(grid, updated, targetX, targetY);
    return true;
}

/*
 * trySwapParticle swaps source with destination if destination is lighter.
 */
static bool trySwapParticle(Grid *grid, int fromX, int fromY, int toX, int toY, bool updated[GRID_HEIGHT][GRID_WIDTH]) {
    if (!inBounds(grid, toX, toY) || !inBounds(grid, fromX, fromY)) {
        return false;
    }

    Particle destination = getParticle(grid, toX, toY);
    Particle source = getParticle(grid, fromX, fromY);

    // Only allow sand to swap with water (sand is denser)
    if (source.type == PARTICLE_SAND && destination.type == PARTICLE_WATER) {
        // Swap particles
        setParticle(grid, toX, toY, source);
        setParticle(grid, fromX, fromY, destination);
        markUpdated(grid, updated, toX, toY);
        markUpdated(grid, updated, fromX, fromY);
        return true;
    }
    
    // Acid dissolves other particles gradually.
    if (source.type == PARTICLE_ACID) {
        return tryDissolveWithAcid(grid, fromX, fromY, toX, toY, updated);
    }
    
    // Fire burns wood
    if (source.type == PARTICLE_FIRE && destination.type == PARTICLE_WOOD) {
        // Convert wood to fire with some randomness
        if (rand() % 10 < 2) {
            setParticle(grid, toX, toY, createParticle(PARTICLE_FIRE));
            markUpdated(grid, updated, toX, toY);
        }
        return true;
    }

    return false;
}

/*
 * updateSand applies sand movement rules for one cell.
 * Sand sinks through water (density simulation).
 */
static void updateSand(Grid *grid, int x, int y, bool updated[GRID_HEIGHT][GRID_WIDTH]) {
    int direction;

    // Try to move down into water or empty
    if (tryMoveParticle(grid, x, y, x, y + 1, updated)) {
        return;
    }
    
    // Try to swap with water (sand sinks)
    if (trySwapParticle(grid, x, y, x, y + 1, updated)) {
        return;
    }

    // Try to move diagonally
    direction = (rand() % 2 == 0) ? -1 : 1;
    if (tryMoveParticle(grid, x, y, x + direction, y + 1, updated)) {
        return;
    }
    
    // Try diagonal swap with water
    if (trySwapParticle(grid, x, y, x + direction, y + 1, updated)) {
        return;
    }

    // Try other diagonal
    tryMoveParticle(grid, x, y, x - direction, y + 1, updated);
}

/*
 * updateWater applies water movement rules for one cell.
 * Water flows down and sideways through other water.
 */
static void updateWater(Grid *grid, int x, int y, bool updated[GRID_HEIGHT][GRID_WIDTH]) {
    int direction;
    bool sandAbove = false;

    if (inBounds(grid, x, y - 1)) {
        Particle above = getParticle(grid, x, y - 1);
        sandAbove = (above.type == PARTICLE_SAND);
    }

    // Give sand-above interactions priority to avoid water and sand
    // both falling in the same simulation step.
    if (!sandAbove) {
        // Try to move down
        if (tryMoveParticle(grid, x, y, x, y + 1, updated)) {
            return;
        }
    }

    // Try to move sideways (water flows through water)
    direction = (rand() % 2 == 0) ? -1 : 1;
    if (tryMoveParticle(grid, x, y, x + direction, y, updated)) {
        return;
    }
    
    if (!sandAbove) {
        // Try to move sideways down (diagonal)
        if (tryMoveParticle(grid, x, y, x + direction, y + 1, updated)) {
            return;
        }

        // Try other diagonal
        tryMoveParticle(grid, x, y, x - direction, y + 1, updated);
    }
}

/*
 * updateFire applies fire movement rules.
 * Fire rises, flickers, and has limited lifetime.
 */
static void updateFire(Grid *grid, int x, int y, bool updated[GRID_HEIGHT][GRID_WIDTH]) {
    // Only decrement lifetime occasionally to prevent issues
    if (rand() % 10 == 0) {
        Particle fire = getParticle(grid, x, y);
        if (fire.lifetime > 0) {
            fire.lifetime--;
            if (fire.lifetime <= 0) {
                // Turn into smoke
                setParticle(grid, x, y, createParticle(PARTICLE_SMOKE));
                markUpdated(grid, updated, x, y);
                return;
            }
            setParticle(grid, x, y, fire);
        }
    }
    
    // Fire can burn wood above it
    Particle above = getParticle(grid, x, y - 1);
    if (above.type == PARTICLE_WOOD && rand() % 10 < 2) {
        setParticle(grid, x, y - 1, createParticle(PARTICLE_FIRE));
        markUpdated(grid, updated, x, y - 1);
    }
    
    // Try to move up (fire rises)
    int direction = (rand() % 2 == 0) ? -1 : 1;
    if (tryMoveParticle(grid, x, y, x, y - 1, updated)) {
        return;
    }
    
    // Try diagonal up
    if (tryMoveParticle(grid, x, y, x + direction, y - 1, updated)) {
        return;
    }
    
    // Flicker - create smoke occasionally
    if (rand() % 30 < 1) {
        setParticle(grid, x, y, createParticle(PARTICLE_SMOKE));
        markUpdated(grid, updated, x, y);
    }
}

/*
 * updateSmoke applies smoke movement rules.
 * Smoke rises and slowly dissipates.
 */
static void updateSmoke(Grid *grid, int x, int y, bool updated[GRID_HEIGHT][GRID_WIDTH]) {
    // Only decrement lifetime occasionally
    if (rand() % 10 == 0) {
        Particle smoke = getParticle(grid, x, y);
        if (smoke.lifetime > 0) {
            smoke.lifetime--;
            if (smoke.lifetime <= 0) {
                // Dissipate
                setParticle(grid, x, y, createEmptyParticle());
                markUpdated(grid, updated, x, y);
                return;
            }
            setParticle(grid, x, y, smoke);
        }
    }
    
    // Try to move up
    if (tryMoveParticle(grid, x, y, x, y - 1, updated)) {
        return;
    }
    
    // Try sideways movement
    int direction = (rand() % 2 == 0) ? -1 : 1;
    if (tryMoveParticle(grid, x, y, x + direction, y, updated)) {
        return;
    }
    
    // Try diagonal up
    tryMoveParticle(grid, x, y, x + direction, y - 1, updated);
}

/*
 * updateAcid applies acid movement rules.
 * Acid flows down and destroys other particles on contact.
 */
static void updateAcid(Grid *grid, int x, int y, bool updated[GRID_HEIGHT][GRID_WIDTH]) {
    int direction;
    Particle above;

    // Keep acid in place while it corrodes stone/wood directly above.
    if (inBounds(grid, x, y - 1)) {
        above = getParticle(grid, x, y - 1);
        if (above.type == PARTICLE_STONE || above.type == PARTICLE_WOOD) {
            trySwapParticle(grid, x, y, x, y - 1, updated);
            return;
        }
    }
    
    // Try to move down
    if (tryMoveParticle(grid, x, y, x, y + 1, updated)) {
        return;
    }
    
    // Try to destroy below
    if (trySwapParticle(grid, x, y, x, y + 1, updated)) {
        return;
    }

    // Try sideways movement
    direction = (rand() % 2 == 0) ? -1 : 1;
    if (tryMoveParticle(grid, x, y, x + direction, y, updated)) {
        return;
    }
    
    // Try diagonal down
    if (tryMoveParticle(grid, x, y, x + direction, y + 1, updated)) {
        return;
    }

    // If movement is blocked, react with nearby particles.
    if (trySwapParticle(grid, x, y, x, y - 1, updated)) {
        return;
    }
    if (trySwapParticle(grid, x, y, x + direction, y, updated)) {
        return;
    }
    trySwapParticle(grid, x, y, x - direction, y, updated);
}

/*
 * updatePhysics applies one simulation step to the whole grid.
 * Optimized to skip empty cells early.
 */
void updatePhysics(Grid *grid) {
    bool updated[GRID_HEIGHT][GRID_WIDTH] = { false };
    int x;
    int y;

    for (y = grid->height - 1; y >= 0; y--) {
        for (x = 0; x < grid->width; x++) {
            // Get particle once and check if empty first
            Particle current = grid->cells[y][x];
            
            // Skip empty cells early - huge performance win
            if (current.type == PARTICLE_EMPTY) {
                continue;
            }
            
            // Skip already updated cells
            if (updated[y][x]) {
                continue;
            }

            if (current.type == PARTICLE_STONE || current.type == PARTICLE_WOOD) {
                continue;
            }

            if (current.type == PARTICLE_SAND) {
                updateSand(grid, x, y, updated);
            } else if (current.type == PARTICLE_WATER) {
                updateWater(grid, x, y, updated);
            } else if (current.type == PARTICLE_FIRE) {
                updateFire(grid, x, y, updated);
            } else if (current.type == PARTICLE_SMOKE) {
                updateSmoke(grid, x, y, updated);
            } else if (current.type == PARTICLE_ACID) {
                updateAcid(grid, x, y, updated);
            }
        }
    }
}
