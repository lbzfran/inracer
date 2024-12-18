#include <stdio.h>
#include "include/raylib.h"
#include "include/raymath.h"
#include "include/constants.h"
#include "include/base.h"
#include <stdlib.h>

//typedef struct entity Entity;
typedef struct car { // in relation to a car.
    Vector2         position;
    f32             rotation;
    Vector2         size;
    Vector2         direction;

    f32     speed;
    f32     maxSpeed;
    f32     rotationSpeed;
    f32     friction;

    bool isRotating;
    bool isMoving;
    bool isAccelerating;
    bool isReversing;
    bool isBreaking;

    // Gear Shift
    i8 gear;
    bool isGearShifting;
    bool isGearReady;
    bool gearDirectionUp;
    u16 gearCooldown;
    u16 gearFullCooldown;
} E_Car;

E_Car* car_create(f32 pos_start_x, f32 pos_start_y) {
    E_Car *temp = malloc(sizeof(E_Car));

    temp->position.x = pos_start_x;
    temp->position.y = pos_start_y;
    temp->speed = 0.0f;

    temp->gear = 0;
    temp->gearCooldown = 0;
    temp->gearFullCooldown = 100;

    temp->size.x = 40.0f;
    temp->size.y = 80.0f;

    temp->direction.x = 40.0f;
    temp->direction.y = 80.0f;

    temp->rotation = 0.0f;
    temp->rotationSpeed = 5.0f;
    temp->friction = 1.0f;
    temp->maxSpeed = 2.0f;
    // states
    temp->isRotating = false;
    temp->isMoving = false;
    temp->isAccelerating = false;
    temp->isReversing = false;
    temp->isBreaking = false;

    temp->isGearShifting = false;
    temp->isGearReady = true;
    temp->gearDirectionUp = false;

    return temp;
}

void input(bool *pause, E_Car* Player) {
    // handles all inputs
    if (IsKeyPressed(KEY_SPACE))
        *pause = !(*pause);

    if (IsKeyDown(KEY_Q)) {
        Player->isGearShifting = true;
        Player->gearDirectionUp = false;
    }
    else if (IsKeyDown(KEY_E)) {
        Player->isGearShifting = true;
        Player->gearDirectionUp = true;
    }

    if (IsKeyDown(KEY_D)) {
        Player->direction.x = 1;
    }
    else if (IsKeyDown(KEY_A)) {
        Player->direction.x = -1;
    }
    else Player->direction.x = 0;


    if (IsKeyDown(KEY_S)) {
        Player->isBreaking = true;
        Player->isAccelerating = false;
    }
    else if (IsKeyDown(KEY_W)) {
        Player->isAccelerating = true;
    }
    else {
        Player->isAccelerating = false;
    }

    if (Player->direction.x != 0)
        Player->isRotating = true;
    else
        Player->isRotating = false;

    if (Player->isAccelerating)
        Player->isMoving = true;
    else
        Player->isMoving = false;
};

void update() {
    // runs all updates
}


int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "r55");

    // SETUP
    int framesCounter = 0;
    bool pause = 0;
    E_Car* Player = car_create(100.0f, 200.0f);

    SetTargetFPS(TARGET_FPS);
    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        // INPUT
        input(&pause, Player);

        // UPDATE
        if (!pause) {
            Player->friction = 1.0f;
            if (Player->isRotating AND ((Player->isMoving AND Player->gear != 0) OR Player->speed > 0)) {
                // only allow turning while moving
                Player->rotation += Player->direction.x * Player->rotationSpeed * 0.5;
                if (Player->rotation >= 360) {
                    Player->rotation = 0;
                };
                Player->friction = 1.0 / Player->gear;
            }
            /*else if (isRotating) {*/
            /*    playerRotation += direction_x * playerRotationSpeed;*/
            /*    if (playerRotation >= 360) {*/
            /*        playerRotation = 0;*/
            /*    };*/
            /*}*/

            if (Player->isMoving AND Player->isAccelerating AND Player->gear != 0 AND Player->speed < Player->maxSpeed * Player->gear) {
                Player->speed += 1.0 * dt;
            } else if (Player->isMoving AND Player->gear == -1) { // reversing
                Player->speed -= 1.0 * dt;
                if (Player->speed <= Player->maxSpeed * Player->gear) {
                    Player->speed = Player->maxSpeed * Player->gear;
                }
            } else if (Player->speed > 0 AND Player->isBreaking) {
                Player->speed -= 4.0 * dt;
                if (Player->speed < 0) {
                    Player->speed = 0;
                }
            } else {
                //printf("%f\n", Player->speed);
                Player->speed -= (1 + Player->friction) * dt;
                if (Player->speed <= 0) {
                    Player->speed = 0;
                }
            }
            //float magnitude = sqrt(direction_x * direction_x + direction_y * direction_y);


            // Movement

            // Player Direction
            float rads = PI * Player->rotation / 180;
            float normalized_x = 1 * cosf(rads);
            float normalized_y = 1 * sinf(rads);

            // Update Position
            Player->position.x += (Player->speed) * normalized_x;
            Player->position.y += (Player->speed) * normalized_y;


            // Gear Control
            if (Player->isGearShifting) {
                /*printf("gear dir up? %d\n", Player->gearDirectionUp);*/
                /*printf("gear ready? %d\n", Player->isGearReady);*/
                /*printf("gear < 5? %d\n", Player->gear);*/
                if (Player->gearDirectionUp AND Player->isGearReady AND Player->gear < 5) {
                    Player->gear++;
                    Player->isGearReady = !Player->isGearReady;
                    Player->gearCooldown = Player->gearFullCooldown;
                }
                else if (!Player->gearDirectionUp AND Player->isGearReady AND Player->gear > -1) {
                    Player->gear--;
                    Player->isGearReady = !Player->isGearReady;
                    Player->gearCooldown = Player->gearFullCooldown;
                }
                Player->isGearShifting = false;
            }

            if (!Player->isGearReady AND Player->gearCooldown > 0) {
                Player->gearCooldown -= 1 * dt;
                if (Player->gearCooldown == 0) {
                    printf("Gear ready to shift.\n");
                    Player->isGearReady = !Player->isGearReady;
                }
            }
        }
        else framesCounter++;


        // RENDER
        BeginDrawing();
            ClearBackground(RAYWHITE);

            //Draw(ballPosition, (float)(ballRadius), MAROON);
            //DrawRectangleV(Player.position, playerSize, MAROON);
            Rectangle PlayerRect = {
                .x = Player->position.x,
                .y = Player->position.y,
                .width = Player->size.y,
                .height = Player->size.x,
            };
            Vector2 PlayerOrigin = {
                .x = Player->size.y / 2,
                .y = Player->size.x / 2,
            };
            DrawRectanglePro(PlayerRect, PlayerOrigin, Player->rotation, MAROON);

            char gearDisplay[50];
            char speedDisplay[50];
            char posDisplay[50];
            snprintf(gearDisplay, sizeof(gearDisplay), "Gear: %d", Player->gear);
            snprintf(speedDisplay, sizeof(speedDisplay), "Speed: %f", Player->speed);
            snprintf(posDisplay, sizeof(posDisplay), "Position: %f %f", Player->position.x, Player->position.y);

            DrawText(gearDisplay, GetScreenWidth() - 200, GetScreenHeight() - 100, 20, LIGHTGRAY);
            DrawText(speedDisplay, GetScreenWidth() - 200, GetScreenHeight() - 80, 20, LIGHTGRAY);
            DrawText(posDisplay, GetScreenWidth() - 400, GetScreenHeight() - 60, 20, LIGHTGRAY);

            if (pause AND ((framesCounter/30)%2)) DrawText("PAUSED", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 , 30, GRAY);
            DrawFPS(10,10);
        EndDrawing();
    }
    CloseWindow();

    return 0;
}
