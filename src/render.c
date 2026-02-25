#include "render.h"

#include "raylib.h"

/*
 * particleColor returns the render color for one particle type with variation.
 */
static Color particleColor(ParticleType type, uint8_t variant) {
    int v = (int)variant * 4;  // Color variation intensity
    
    if (type == PARTICLE_SAND) {
        // Sandy brown with variation
        return (Color){194 + v/3, 178 - v/4, 128 - v/2, 255};
    }

    if (type == PARTICLE_WATER) {
        // Blue with variation
        return (Color){64 + v/2, 144 - v/4, 255 - v/4, 200};
    }

    if (type == PARTICLE_STONE) {
        // Gray with variation
        return (Color){110 + v/3, 110 + v/4, 110 + v/3, 255};
    }
    
    if (type == PARTICLE_FIRE) {
        // Orange/yellow/red fire colors
        int fireVariant = variant % 8;
        if (fireVariant < 3) {
            return (Color){255, 100 + fireVariant * 30, 0, 255};
        } else if (fireVariant < 6) {
            return (Color){255, 180, 50, 255};
        } else {
            return (Color){255, 50, 0, 255};
        }
    }
    
    if (type == PARTICLE_SMOKE) {
        // Gray smoke with variation
        uint8_t smokeGray = 80 + variant * 6;
        return (Color){smokeGray, smokeGray, smokeGray, 180};
    }
    
    if (type == PARTICLE_WOOD) {
        // Brown wood with variation
        return (Color){139 - v/4, 69 + v/4, 19, 255};
    }
    
    if (type == PARTICLE_ACID) {
        // Green acid with variation
        return (Color){50 + v/2, 200 + v/4, 50, 230};
    }

    return (Color){18, 24, 32, 255};
}

/*
 * renderGrid draws all grid cells using colors by particle type.
 */
void renderGrid(const Grid *grid) {
    int x;
    int y;

    for (y = 0; y < grid->height; y++) {
        for (x = 0; x < grid->width; x++) {
            Particle cell = getParticle(grid, x, y);
            
            if (cell.type == PARTICLE_EMPTY) {
                continue;  // Skip drawing empty cells (background shows through)
            }
            
            Color color = particleColor(cell.type, cell.colorVariant);
            int drawX = x * CELL_SIZE;
            int drawY = y * CELL_SIZE;

            DrawRectangle(drawX, drawY, CELL_SIZE, CELL_SIZE, color);
        }
    }
}
