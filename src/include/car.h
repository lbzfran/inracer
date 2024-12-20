#ifndef CAR_H
#define CAR_H

#include "base.h"
#include "raylib.h"

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
         //
#endif // CAR_H
