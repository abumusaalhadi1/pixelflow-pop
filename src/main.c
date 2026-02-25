#include "raylib.h"

#include <stdlib.h>
#include <time.h>

#include "grid.h"
#include "physics.h"
#include "render.h"
#include "ui.h"

int main(void) {
    const int screenWidth = GRID_WIDTH * CELL_SIZE;
    const int screenHeight = GRID_HEIGHT * CELL_SIZE;
    Grid grid;
    UIState ui;

    InitWindow(screenWidth, screenHeight, "PixelFlow");
    SetTargetFPS(60);
    srand((unsigned int)time(NULL));
    initGrid(&grid);
    initUI(&ui);

    while (!WindowShouldClose()) {
        handleUIInput(&ui, &grid);
        
        // Only update physics when not paused
        if (!ui.isPaused) {
            updatePhysics(&grid);
        }

        BeginDrawing();
        ClearBackground((Color){18, 24, 32, 255});
        renderGrid(&grid);
        drawUI(&ui);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
