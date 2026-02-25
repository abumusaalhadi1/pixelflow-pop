#include "ui.h"

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
        ui->selectedType = PARTICLE_SMOKE;
    } else if (IsKeyPressed(KEY_SIX)) {
        ui->selectedType = PARTICLE_WOOD;
    } else if (IsKeyPressed(KEY_SEVEN)) {
        ui->selectedType = PARTICLE_ACID;
    } else if (IsKeyPressed(KEY_ZERO)) {
        ui->selectedType = PARTICLE_EMPTY;
    }
    
    // Brush size controls with bracket keys
    if (IsKeyPressed(KEY_LEFT_BRACKET)) {
        if (ui->brushSize > 1) ui->brushSize--;
    } else if (IsKeyPressed(KEY_RIGHT_BRACKET)) {
        if (ui->brushSize < 6) ui->brushSize++;
    }
}

/*
 * paintAtMouse writes particles into cells under the mouse brush.
 */
static void paintAtMouse(const UIState *ui, Grid *grid) {
    Vector2 mouse = GetMousePosition();
    int centerX = (int)(mouse.x / CELL_SIZE);
    int centerY = (int)(mouse.y / CELL_SIZE);
    int brushRadius = ui->brushSize;

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        int dx, dy;
        for (dy = -brushRadius; dy <= brushRadius; dy++) {
            for (dx = -brushRadius; dx <= brushRadius; dx++) {
                if (dx*dx + dy*dy <= brushRadius*brushRadius) {
                    setParticle(grid, centerX + dx, centerY + dy, createParticle(ui->selectedType));
                }
            }
        }
    }

    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
        int dx, dy;
        for (dy = -brushRadius; dy <= brushRadius; dy++) {
            for (dx = -brushRadius; dx <= brushRadius; dx++) {
                if (dx*dx + dy*dy <= brushRadius*brushRadius) {
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
    
    if (type == PARTICLE_SMOKE) {
        return "SMOKE";
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
    ui->brushSize = 1;
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
    DrawText("1:SAND 2:WATER 3:STONE 4:FIRE 5:SMOKE 6:WOOD 7:ACID", 20, 18, 16, RAYWHITE);
    DrawText("0:ERASE C:CLEAR SPACE:PAUSE [ / ] :Brush Size", 20, 38, 16, RAYWHITE);
    
    // Selected particle
    DrawText(TextFormat("Selected: %s", selectedTypeLabel(ui->selectedType)), 20, 60, 18, YELLOW);
    
    // Brush size
    DrawText(TextFormat("Brush: %d", ui->brushSize), 250, 60, 18, (Color){100, 200, 255, 255});
    
    // Pause indicator
    if (ui->isPaused) {
        DrawText("PAUSED", 400, 60, 18, (Color){255, 100, 100, 255});
    }
    
    // FPS
    int fps = GetFPS();
    DrawText(TextFormat("FPS: %d", fps), 480, 18, 16, (Color){100, 255, 100, 255});
}
