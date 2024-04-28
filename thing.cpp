#include <cstdlib>
#include <ctime>
#include <iostream>
#include <utility>

#include <raylib.h>

constexpr int CELL_SIZE = 8;  // Pixels.
constexpr int GRID_WIDTH = 200;
constexpr int GRID_HEIGHT = 140;
constexpr int SCREEN_WIDTH = GRID_WIDTH * CELL_SIZE + 1;
constexpr int SCREEN_HEIGHT = GRID_HEIGHT * CELL_SIZE + 1;
constexpr int FPS_TARGET = 60;
constexpr int CPS_TARGET = 30; // cycles per second
constexpr int GPC = 1;  // generations per cycle
constexpr int FRAMES_PER_CYCLE = FPS_TARGET / CPS_TARGET;
constexpr char TITLE[] = "thing";

void seed_rng() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

void clear_world(bool (&world)[GRID_HEIGHT+2][GRID_WIDTH+2]) {
    for (int i = 1; i <= GRID_HEIGHT;  ++i) {
        for (int j = 1; j <= GRID_WIDTH;  ++j) {
            world[i][j] = false;
        }
    }
}

void populate_world(bool (&world)[GRID_HEIGHT+2][GRID_WIDTH+2]) {
    for (int i = 1; i <= GRID_HEIGHT;  ++i) {
        for (int j = 1; j <= GRID_WIDTH;  ++j) {
            world[i][j] = 0 == std::rand() % 5;
        }
    }
}

void initialize_graphics() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, TITLE);
    SetTargetFPS(FPS_TARGET);
}

int main() {
    // Extra "always false" end rows and columns, for efficiency.
    bool cell_state[2][GRID_HEIGHT+2][GRID_WIDTH+2] {};
    int buf_now = 0;
    int buf_next = 1;
    unsigned long frame = 0;
    bool prev_clicked = false;
    int prev_click_i = 0;
    int prev_click_j = 0;
    bool paused = false;

    seed_rng();
    populate_world(cell_state[buf_now]);
    initialize_graphics();


    /**** MAIN LOOP ****/

    while (!WindowShouldClose()) {

        /**** RENDER ****/

        BeginDrawing();

        ClearBackground(BLACK);
        for (int i = 1; i <= GRID_HEIGHT;  ++i) {
            for (int j = 1; j <= GRID_WIDTH;  ++j) {
                Color color = cell_state[buf_now][i][j] ? GREEN : DARKBLUE ;
                DrawRectangle((j-1) * CELL_SIZE + 1, (i-1) * CELL_SIZE + 1, CELL_SIZE - 1, CELL_SIZE - 1, color);
            }
        }

        DrawFPS(10, 10);
        EndDrawing();
        ++frame;

        /**** USER INPUT ****/

        if (IsKeyPressed(KEY_SPACE)) {
            paused = !paused;
        }

        if (IsKeyPressed(KEY_R)) {
            populate_world(cell_state[buf_now]);
        }

        if (IsKeyPressed(KEY_C)) {
            clear_world(cell_state[buf_now]);
        }

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            int i = GetMouseY() / CELL_SIZE + 1;
            int j = GetMouseX() / CELL_SIZE + 1;
            // Don't re-click in next frame unless user dragged mouse to another cell.
            if (!prev_clicked || prev_click_i != i || prev_click_j != j) {
                if (i >= 1 && j >= 1 && i <= GRID_HEIGHT && j <= GRID_WIDTH) {
                    cell_state[buf_now][i][j] = !cell_state[buf_now][i][j];
                }
            }
            prev_clicked = true;
            prev_click_i = i;
            prev_click_j = j;
        } else {
            prev_clicked = false;
        }

        /****  PHYSICS ****/

        bool run_physics = !paused && (frame % FRAMES_PER_CYCLE == 0);
        if (run_physics) {
             for (int n = 0; n < GPC; ++n){
                for (int i = 1; i <= GRID_HEIGHT;  ++i) {
                    for (int j = 1; j <= GRID_WIDTH;  ++j) {
                        int neighbours_alive = -cell_state[buf_now][i][j];  // LIFE
                        //int neighbours_alive = 0;  // SIERPINSKI
                        for (int imod = -1; imod <= 1; ++imod) {  // LIFE
                        //for (int imod = -1; imod <= -1; ++imod) {  // SIERPINSKI
                            for (int jmod = -1; jmod <= 1; ++jmod) {
                                neighbours_alive += cell_state[buf_now][i+imod][j+jmod] ? 1 : 0;
                            }
                        }
                        cell_state[buf_next][i][j] =  // LIFE
                            (cell_state[buf_now][i][j] && (neighbours_alive == 2 || neighbours_alive == 3)) ||
                            (!cell_state[buf_now][i][j] && (neighbours_alive == 3));
                        //cell_state[buf_next][i][j] = cell_state[buf_now][i][j] || neighbours_alive == 1;  // SIERPINSKI
                    }
                }
                std::swap(buf_now, buf_next);
            }
        }
    }

    CloseWindow();
    return 0;
}
