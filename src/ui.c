#include "ui.h"

#include <stdlib.h>

#include "raylib.h"

/*
 * selectParticleFromKeyboard changes selected particle by number keys.
 */
static void selectParticleFromKeyboard(UIState *ui) {
    if (IsKeyPressed(KEY_ONE)) {
        ui->selectedType = PARTICLE_SAND;
    } else if (IsKeyPressed(KEY_TWO)) {
        ui->selectedType = PARTICLE_WATER;
    } else if (IsKeyPressed(KEY_THREE)) {
        ui->selectedType = PARTICLE_STONE;
    } else if (IsKeyPressed(KEY_FOUR)) {
        ui->selectedType = PARTICLE_FIRE;
    } else if (IsKeyPressed(KEY_FIVE)) {
        ui->selectedType = PARTICLE_WOOD;
    } else if (IsKeyPressed(KEY_SIX)) {
        ui->selectedType = PARTICLE_ACID;
    } else if (IsKeyPressed(KEY_ZERO)) {
        ui->selectedType = PARTICLE_EMPTY;
    }
}

/*
 * paintAtMouse writes particles into cells under the mouse brush.
 */
static void paintAtMouse(const UIState *ui, Grid *grid) {
    if (!IsCursorOnScreen()) {
        return;
    }

    Vector2 mouse = GetMousePosition();
    int centerX = (int)(mouse.x / CELL_SIZE);
    int centerY = (int)(mouse.y / CELL_SIZE);
    const int brushRadius = 1;
    bool leftDown = IsMouseButtonDown(MOUSE_BUTTON_LEFT);
    bool rightDown = IsMouseButtonDown(MOUSE_BUTTON_RIGHT);

    if (!leftDown && !rightDown) {
        return;
    }

    if (!inBounds(grid, centerX, centerY)) {
        return;
    }

    if (leftDown) {
        if (ui->selectedType == PARTICLE_FIRE) {
            // Approximate a 0.5-sized brush on integer cells: center always, plus one
            // adjacent cell half the time.
            setParticle(grid, centerX, centerY, createParticle(ui->selectedType));
            if (rand() % 2 == 0) {
                int dir = rand() % 4;
                int offsetX[4] = {1, -1, 0, 0};
                int offsetY[4] = {0, 0, 1, -1};
                setParticle(
                    grid,
                    centerX + offsetX[dir],
                    centerY + offsetY[dir],
                    createParticle(ui->selectedType));
            }
            return;
        }

        int dx, dy;
        for (dy = -brushRadius; dy <= brushRadius; dy++) {
            for (dx = -brushRadius; dx <= brushRadius; dx++) {
                if (dx * dx + dy * dy <= brushRadius * brushRadius) {
                    setParticle(grid, centerX + dx, centerY + dy, createParticle(ui->selectedType));
                }
            }
        }
    }

    if (rightDown) {
        int dx, dy;
        for (dy = -brushRadius; dy <= brushRadius; dy++) {
            for (dx = -brushRadius; dx <= brushRadius; dx++) {
                if (dx * dx + dy * dy <= brushRadius * brushRadius) {
                    setParticle(grid, centerX + dx, centerY + dy, createEmptyParticle());
                }
            }
        }
    }
}

/*
 * selectedTypeLabel returns a short readable label for UI text.
 */
static const char *selectedTypeLabel(ParticleType type) {
    if (type == PARTICLE_SAND) {
        return "SAND";
    }

    if (type == PARTICLE_WATER) {
        return "WATER";
    }

    if (type == PARTICLE_STONE) {
        return "STONE";
    }
    
    if (type == PARTICLE_FIRE) {
        return "FIRE";
    }
    
    if (type == PARTICLE_WOOD) {
        return "WOOD";
    }
    
    if (type == PARTICLE_ACID) {
        return "ACID";
    }

    return "EMPTY";
}

/*
 * initUI sets default UI state.
 */
void initUI(UIState *ui) {
    ui->selectedType = PARTICLE_SAND;
    ui->isPaused = false;
}

/*
 * handleUIInput reads keyboard and mouse input and paints particles.
 */
void handleUIInput(UIState *ui, Grid *grid) {
    // Pause/Play toggle
    if (IsKeyPressed(KEY_SPACE)) {
        ui->isPaused = !ui->isPaused;
    }
    
    selectParticleFromKeyboard(ui);
    paintAtMouse(ui, grid);

    if (IsKeyPressed(KEY_C)) {
        resetGrid(grid);
    }
}

/*
 * drawUI draws simple controls and current particle selection.
 */
void drawUI(const UIState *ui) {
    // Background panel
    DrawRectangle(10, 10, 540, 90, (Color){0, 0, 0, 180});
    
    // Controls text
    DrawText("1:SAND 2:WATER 3:STONE 4:FIRE 5:WOOD 6:ACID", 20, 18, 16, RAYWHITE);
    DrawText("0:ERASE C:CLEAR SPACE:PAUSE", 20, 38, 16, RAYWHITE);
    
    // Selected particle
    DrawText(TextFormat("Selected: %s", selectedTypeLabel(ui->selectedType)), 20, 60, 18, YELLOW);
    
    // Pause indicator
    if (ui->isPaused) {
        DrawText("PAUSED", 260, 60, 18, (Color){255, 100, 100, 255});
    }
    
    // FPS
    int fps = GetFPS();
    DrawText(TextFormat("FPS: %d", fps), 480, 18, 16, (Color){100, 255, 100, 255});
}
