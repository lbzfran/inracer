#include "include/raylib.h"
#include "include/raymath.h"
#include "include/constants.h"

typedef struct entity Entity;

struct bullet {
    float radius;
};

void move_entity(int direction) {

}


bool isMoving;
int direction_x = 0, direction_y = 0;

void input(bool *pause) {
    // handles all inputs
    if (IsKeyPressed(KEY_SPACE))
        *pause = !(*pause);


    if (IsKeyDown(KEY_D)) {
        direction_x = 1;
    }
    else if (IsKeyDown(KEY_A)) {
        direction_x = -1;
    }
    else direction_x = 0;

    if (IsKeyDown(KEY_S)) {
        direction_y = 1;
    }
    else if (IsKeyDown(KEY_W)) {
        direction_y = -1;
    }
    else direction_y = 0;

    if (direction_x != 0 || direction_y != 0)
        isMoving = true;
    else
        isMoving = false;
};

void update() {
    // runs all updates
}


int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "r55");

    // SETUP
    Vector2 ballPosition = { SCREEN_WIDTH/2.0f, SCREEN_HEIGHT/2.0f };
    float ballSpeed = 4.0f;
    int ballRadius = 20;

    int framesCounter = 0;

    int radiusInc = 1;

    bool pause = 0;

    SetTargetFPS(TARGET_FPS);
    while (!WindowShouldClose()) {

        // INPUT
        input(&pause);

        // UPDATE
        if (!pause) {
            if (ballRadius < 20) {
                radiusInc = 1;
            } else radiusInc = -1;

            if ((framesCounter % 30) == 0)
                ballRadius += 2 * radiusInc;

            if (isMoving) {
                float magnitude = sqrt(direction_x * direction_x + direction_y * direction_y);

                float normalized_x = direction_x / magnitude;
                float normalized_y = direction_y / magnitude;

                ballPosition.x += ballSpeed * normalized_x;
                ballPosition.y += ballSpeed * normalized_y;
            }
        }
        else framesCounter++;


        // RENDER
        BeginDrawing();
            ClearBackground(RAYWHITE);

            DrawCircleV(ballPosition, (float)(ballRadius), MAROON);
            DrawText("Press SPACE to PAUSE!", 10, GetScreenHeight() -25, 20, LIGHTGRAY);

            if (pause && ((framesCounter/30)%2)) DrawText("PAUSED", ballPosition.x - 50, ballPosition.y - 50, 30, GRAY);
            DrawFPS(10,10);
        EndDrawing();
    }
    CloseWindow();

    return 0;
}
