#include <stdio.h>
#include "include/raylib.h"
#include "include/raymath.h"
#include "include/constants.h"
#include "include/base.h"

//typedef struct entity Entity;

struct movement {
    Vector2 position;
    float speed;
};

struct movement Player = {
    .position = { SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f },
    .speed = 0.0f,
};

struct movement_car {
    bool isRotating;
    bool isMoving;
    bool isAccelerating;
    bool isReversing;
    bool isBreaking;

    // Gear Shift
    int playerGear;
    bool isGearShifting;
    bool isGearReady;
    bool gearDirectionUp;
    int gearCooldown;
    int gearFullCooldown;
};

bool isRotating = false;
bool isMoving = false;
bool isAccelerating = false;
bool isReversing = false;
bool isBreaking = false;

// Gear Shift
int playerGear = 0;
bool isGearShifting = false;
bool isGearReady = true;
bool gearDirectionUp = false;
int gearCooldown = 0;
int gearFullCooldown = 30;

int direction_x = 0, direction_y = 0;

void input(bool *pause) {
    // handles all inputs
    if (IsKeyPressed(KEY_SPACE))
        *pause = !(*pause);

    if (IsKeyDown(KEY_Q)) {
        isGearShifting = true;
        gearDirectionUp = false;
    }
    else if (IsKeyDown(KEY_E)) {
        isGearShifting = true;
        gearDirectionUp = true;
    }

    if (IsKeyDown(KEY_D)) {
        direction_x = 1;
    }
    else if (IsKeyDown(KEY_A)) {
        direction_x = -1;
    }
    else direction_x = 0;


    if (IsKeyDown(KEY_S)) {
        isBreaking = true;
        isAccelerating = false;
    }
    else if (IsKeyDown(KEY_W)) {
        isAccelerating = true;
    }
    else {
        isAccelerating = false;
    }

    if (direction_x != 0)
        isRotating = true;
    else
        isRotating = false;

    if (isAccelerating)
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
    int framesCounter = 0;
    bool pause = 0;

    float playerRotation = 0.0f;
    float playerRotationSpeed = 5.0f;
    float playerFriction = 1.0f;
    float playerMaxSpeed = 2.0f;
    Vector2 playerSize = { 40.0f, 80.0f };

    SetTargetFPS(TARGET_FPS);
    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        // INPUT
        input(&pause);

        // UPDATE
        if (!pause) {
            playerFriction = 1.0f;
            if (isRotating AND ((isMoving AND playerGear != 0) OR Player.speed > 0)) {
                // only allow turning while moving
                playerRotation += direction_x * playerRotationSpeed * 0.5;
                if (playerRotation >= 360) {
                    playerRotation = 0;
                };
                playerFriction = 1.0 / playerGear;
            }
            /*else if (isRotating) {*/
            /*    playerRotation += direction_x * playerRotationSpeed;*/
            /*    if (playerRotation >= 360) {*/
            /*        playerRotation = 0;*/
            /*    };*/
            /*}*/

            if (isMoving AND isAccelerating AND playerGear != 0) {
                Player.speed += 1.0 * dt;
                if (Player.speed >= playerMaxSpeed * playerGear) {
                    // NOTE: downshifting sets speed lower instantly.
                    Player.speed = playerMaxSpeed * playerGear;
                }
            } else if (isMoving AND playerGear == -1) { // reversing
                Player.speed -= 1.0 * dt;
                if (Player.speed <= playerMaxSpeed * playerGear) {
                    Player.speed = playerMaxSpeed * playerGear;
                }
            } else if (Player.speed > 0 AND isBreaking) {
                Player.speed -= 4.0 * dt;
                if (Player.speed < 0) {
                    Player.speed = 0;
                }
            } else {
                Player.speed -= (1 + playerFriction) * dt;
                //printf("%f\n", Player.speed);
                if (Player.speed < 0) {
                    Player.speed = 0;
                }
            }
            //float magnitude = sqrt(direction_x * direction_x + direction_y * direction_y);


            // Movement

            // Player Direction
            float rads = PI * playerRotation / 180;
            float normalized_x = 1 * cosf(rads);
            float normalized_y = 1 * sinf(rads);

            // Update Position
            Player.position.x += (Player.speed) * normalized_x;
            Player.position.y += (Player.speed) * normalized_y;




            // Gear Control
            if (isGearShifting) {
                if (gearDirectionUp AND isGearReady AND playerGear < 5) {
                    playerGear++;
                    isGearReady = !isGearReady;
                    gearCooldown = gearFullCooldown;
                }
                else if (!gearDirectionUp AND isGearReady AND playerGear > -1) {
                    playerGear--;
                    isGearReady = !isGearReady;
                    gearCooldown = gearFullCooldown;
                }
                isGearShifting = false;
            }

            if (!isGearReady AND gearCooldown > 0) {
                gearCooldown -= 1 * dt;
                if (gearCooldown == 0) {
                    printf("Gear ready to shift.\n");
                    isGearReady = !isGearReady;
                }
            }
        }
        else framesCounter++;


        // RENDER
        BeginDrawing();
            ClearBackground(RAYWHITE);

            //Draw(ballPosition, (float)(ballRadius), MAROON);
            //DrawRectangleV(Player.position, playerSize, MAROON);
            Rectangle playerRect = {
                .x = Player.position.x,
                .y = Player.position.y,
                .width = playerSize.y,
                .height = playerSize.x,
            };
            Vector2 playerOrigin = {
                .x = playerSize.y / 2,
                .y = playerSize.x / 2,
            };
            DrawRectanglePro(playerRect, playerOrigin, playerRotation, MAROON);

            char gearDisplay[50];
            char speedDisplay[50];
            snprintf(gearDisplay, sizeof(gearDisplay), "Gear: %d", playerGear);
            snprintf(speedDisplay, sizeof(speedDisplay), "Speed: %f", Player.speed);

            DrawText(gearDisplay, GetScreenWidth() - 200, GetScreenHeight() - 100, 20, LIGHTGRAY);
            DrawText(speedDisplay, GetScreenWidth() - 200, GetScreenHeight() - 80, 20, LIGHTGRAY);

            if (pause AND ((framesCounter/30)%2)) DrawText("PAUSED", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 , 30, GRAY);
            DrawFPS(10,10);
        EndDrawing();
    }
    CloseWindow();

    return 0;
}
