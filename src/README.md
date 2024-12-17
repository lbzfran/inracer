
# Introduction
This is an attempt at a fully-featured car drifting simulator.

# Concepts

## Acceleration and Speed

- Throttle (engine power): horsepower of engine applying to wheels.
- Friction (tire and surface): grip between tire and road.
- Drag: Air resistance that slows car down as it moves faster.

## Handling (Steering and Rotation)

- Steering Angle: angle that car's wheels are turned.
- Turning Radius: car's ability to change direction based on steering input,
influenced by speed, friction, and the car itself.
- Oversteer: when rear tires lose grip more than front tire.

## Car Velocity

v = v_old + a * dt
> v: velocity of the car
> a: applied acceleration
> dt: time passed

## Friction

F_friction = \mu * N
> F_friction: force from friction
> \mu: coefficient of force
> N: normal force (usually mass times gravity)

## Angular Velocity and Rotation

Rate of change of the car's rotation (or speed at which it turns).
\theta = \theta_old + w * dt
> \theta: rotation
> w: angular velocity
> dt: time passed

## Sliding/Drifting Velocity

v_car = v_car - F_friction * dt

# Implementations

Data being tracked:
- Position:             pos (x, y)
- Velocity:             v   (x, y)
- speed:                s   sqrt(v_x^2 + v_y^2)
                                    // Pythagorean Theorem
- time:                 t

- angle:                th  rads
- steer_input:          st_input    // player input in turning.
- steer_rate:           st_rate     // how fast car is able to turn.
- angular velocity:     w

- throttle:             tr  (0.0 - 1.0)
                                    // user input.
- engine_force:         f_engine
- acceleration:         a

- friction_coeff:       mu
- car_mass:             m           // constant.
- gravity:              g           // constant.
- normal_force:         N   (m * g)

- gear:                 gr
- rpm:                  rpm         // current revolution per min of engine
- max_rpm:              rpm_max
- min_rpm:              rpm_min
- gear_ratios:          gr_ratios   // how much car speeds up per RPM in each gr

- is_drifting:          is_drifting // bool
- drift_factor:         d_factor    // multiplier controlling extent of drift
                                    // (how much rear tires lose grip)

- slip_angle:           th_slip     // angle between car's heading direction
                                    // and its velocity direction.
- max_slip_angle:       th_slip_max // threshold at which car starts drifting.
                                    // When the difference between heading and
                                    // velocity exceeds this angle, drift.
- velocity_dir_angle:   v_dir_th    // angle of the velocity vector.

- friction_front:       f_fr_front  // front tire friction coeff
- friction_rear:        f_fr_rear
- traction_loss:        tc_loss     // threshold at which traction is lost.

- collision_normal:     c_norm (x, y)
                                    // normal vec of a collision (useful for
                                    // determining car's rebound direction).
- collision_detected:   is_colliding
                                    // bool
- impulse_force:        f_impulse   // force applied during a collision.
                                    // changes the car's velocity based on the
                                    // impact.

## Calculations

Position:
> pos_x = pos_x + v_x * dt
> pos_y = pos_y + v_y * dt

Velocity:
> v_x = v_x + a_x * dt
> v_y = v_y + a_y * dt

Angular Velocity:
> w = st_input * st_rate

Rotation:
> th = th + w * dt


Velocity Direction Angle:
> v_dir_th = atan2(v_y, v_x)

Slip Angle:
> th_slip = th - v_dir_th

if th_slip >= th_slip_max, then is_drifting.

Counter-steering Force:
> f_st_counter = inverse_of(th_slip)

if is_drifting:
    reduce f_fr_rear.
    apply counter-steering force.



RPM:
> rpm = s * gr_ratios(gr)

if rpm >= rpm_max, upshift   gear.
if rpm <= rpm_min, downshift gear.

Throttle and Gear-related Acceleration:
> a = tr * f_engine * gr_ratios(gr)

f_engine = tr and car engine characteristics.


Friction:
> f_friction = mu * N

Collision Normal Force:
> f_N = dv/dt

dv is the change in velocity due to the collision.
dt is the duration of the collision.


Velocity Normal:
> v_norm = v_x * c_norm_x + v_y * c_norm_y

Impulse Force:
> f_impulse = -f_N * v_norm
