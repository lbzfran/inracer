#include <stdio.h>
#include <stdlib.h>

#include "include/raylib.h"
#include "include/raymath.h"
#include "include/constants.h"
#include "include/base.h"



//typedef struct entity Entity;
typedef struct car { // in relation to a car.
    Vector2         position;
    f32             rotation; // angle
    Vector2         size;
    Vector2         direction;

    f32             speed;
    f32             maxSpeed;
    f32             rotationRate;
    f32             friction;

    Vector2         velocity;
    f32             angularVelocity;

    f32             mass;
    f32             forceNormal;
    f32             mu;             // friction coeff

    i8              gear;
    u8              rpm;
    // TODO: add back gearRatios;

    f32             slipAngle;
    f32             slipAngleMax;
    f32             velocityDirAngle;

    bool            isSteering;
    bool            isMoving;
    bool            isAccelerating;
    bool            isReversing;
    bool            isBreaking;

    // Gear Shift
    bool            isGearShifting;
    bool            isGearReady;
    bool            gearDirectionUp;
    u16             gearCooldown;
    u16             gearFullCooldown;
} E_Car; // ENTITY_Car

E_Car* Player;

E_Car* car_create(Vector2 pos_start) {
    E_Car *car = malloc(sizeof(E_Car));

    // datas
    car->position.x         = pos_start.x;
    car->position.y         = pos_start.y;

    car->speed              = 0.0f;
    car->maxSpeed           = 2.0f;

    car->gear               = 0;
    car->gearCooldown       = 0;
    car->gearFullCooldown   = 100;

    car->rpm                = 0;

    car->size.x             = 40.0f;
    car->size.y             = 80.0f;

    car->direction.x        = 40.0f;
    car->direction.y        = 80.0f;

    car->velocity.x         = 0.0f;
    car->velocity.y         = 0.0f;
    car->velocityDirAngle   = 0.0f;
    car->angularVelocity    = 0.0f;

    car->mass               = 100;
    car->forceNormal        = car->mass * GRAVITY;


    car->rotation           = 0.0f;
    car->rotationRate       = 5.0f;
    car->friction           = 1.0f;
    car->mu                 = car->friction / car->forceNormal;// friction coeff

    car->slipAngle          = car->rotation - car->velocityDirAngle;
    car->slipAngleMax       = 30.0f;

    // states
    car->isSteering         = false;
    car->isMoving           = false;
    car->isAccelerating     = false;
    car->isReversing        = false;
    car->isBreaking         = false;

    car->isGearShifting     = false;
    car->isGearReady        = true;
    car->gearDirectionUp    = false;

    return car;
}

void input(bool *pause) {
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
        Player->isSteering = true;
    else
        Player->isSteering = false;

    if (Player->isAccelerating)
        Player->isMoving = true;
    else
        Player->isMoving = false;
};

void update(float dt) {
    // runs all updates
    Player->friction = 1.0f;
    if (Player->isSteering AND ((Player->isMoving AND Player->gear != 0) OR Player->speed > 0)) {
        // only allow turning while moving
        Player->rotation += Player->direction.x * Player->rotationRate * 0.5;
        if (Player->rotation >= 360) {
            Player->rotation = 0;
        };
        Player->friction = 1.0 / Player->gear;
    }
    /*else if (isSteering) {*/
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


int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "r55");

    // SETUP

    Camera camera = { 0 };
    camera.position = (Vector3){ 10.0f, 10.0f, 10.0f };
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;



    int framesCounter = 0;
    bool pause = 0;
    Player = car_create((Vector2){100.0f, 200.0f});


    SetTargetFPS(TARGET_FPS);
    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        // INPUT
        input(&pause);

        // UPDATE
        if (!pause) {
            update(dt);
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

            DrawCube;


            // Draw TEXT
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

            DrawFPS(10, 10);
        EndDrawing();
    }
    CloseWindow();

    free(Player);

    return 0;
}
