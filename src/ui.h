#ifndef UI_H
#define UI_H

#include "grid.h"

/*
 * UIState stores current tool settings selected by the user.
 */
typedef struct {
    ParticleType selectedType;
    int brushSize;
    bool isPaused;
} UIState;

/*
 * initUI sets default UI state.
 */
void initUI(UIState *ui);

/*
 * handleUIInput reads keyboard and mouse input and paints particles.
 */
void handleUIInput(UIState *ui, Grid *grid);

/*
 * drawUI draws simple controls and current particle selection.
 */
void drawUI(const UIState *ui);

#endif
