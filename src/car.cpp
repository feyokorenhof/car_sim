#include "../inc/car_sim/car.h"
#include <algorithm>
#include <cmath>
#include <iterator>
#include <stdio.h>
#include <string>

Car::Car(VehicleConfig config)
    : name(config.name), mass(config.mass), peak_torque(config.peak_torque),
      drag(config.drag), frontal_area(config.frontal_area),
      wheel_radius(config.wheel_radius),
      rolling_resistance(config.rolling_resistance),
      final_drive(config.final_drive), gear_efficiency(config.gear_efficiency),
      rpm_limit(config.rpm_limit), max_braking_force(config.max_braking_force),
      gear_ratios(config.gear_ratios), torque_curve(config.torque_curve)

{
}

Car::Car()
    : name(""), mass(0.0f), peak_torque(0.0f), drag(0.0f), frontal_area(0.0f),
      wheel_radius(0.0f), rolling_resistance(0.0f), final_drive(0.0f),
      gear_efficiency(0.0f), rpm_limit(0.0f), max_braking_force(0.0f),
      gear_ratios(0.0f), torque_curve(0.0f)

{
}

std::string Car::GetName() { return name; }

// Wheel speed in rad/s
float Car::GetWheelSpeed() { return velocity / wheel_radius; }
float Car::GetTorque()
{
    float _rpm = GetRPM();

    // Rev limiter
    if (_rpm >= rpm_limit)
        return 0.0f;

    _rpm = std::clamp(_rpm, 1000.f, 6500.f);

    // Out of bounds checks
    if (_rpm <= torque_curve.front().rpm)
        return torque_curve.front().torque;
    if (_rpm >= torque_curve.back().rpm)
        return torque_curve.back().torque;

    // Find next highest value
    auto right = std::upper_bound(torque_curve.begin(), torque_curve.end(),
                                  _rpm, [](float rpm, const TorquePoint &item)
                                  { return rpm < item.rpm; });
    auto left = std::prev(right);

    // Lerp
    float t = (_rpm - left->rpm) / (right->rpm - left->rpm);
    float _torq = left->torque + t * (right->torque - left->torque);
    return _torq;
}

float Car::GetRPM()
{
    float rpm = GetWheelSpeed() * gear_ratios[current_gear - 1] * final_drive *
                (60.0 / (2 * M_PI));
    return rpm;
}

// In Newtons
float Car::GetDriveForce()
{
    return (GetTorque() * throttle * gear_ratios[current_gear - 1] *
            final_drive * gear_efficiency) /
           wheel_radius;
}

float Car::GetAeroDrag()
{
    return 0.5 * 1.2 * drag * frontal_area * pow(velocity, 2);
}

float Car::GetRollingResistance() { return rolling_resistance * mass * 9.81; }

float Car::GetNettoForce()
{
    return GetDriveForce() - GetAeroDrag() - GetRollingResistance() -
           (max_braking_force * brake);
}

float Car::GetAcceleration() { return GetNettoForce() / mass; }

void Car::Accelerate(const float FIXED_DT)
{
    float new_velocity = velocity + GetAcceleration() * FIXED_DT;
    if (new_velocity < 0.0f)
    {
        new_velocity = 0.0f;
    }
    velocity = new_velocity;
}

void Car::SetThrottle(float t) { throttle = t; }
void Car::SetBrake(float b) { brake = b; }
void Car::Shift(int dir)
{
    if (dir == 1)
    {
        current_gear = std::clamp(current_gear + 1, 1, GetHighestGear());
    }
    else if (dir == -1)
    {
        current_gear = std::clamp(current_gear - 1, 1, GetHighestGear());
    }
}
float Car::GetVelocityKmH() { return velocity * 3.6f; }
float Car::GetThrottle() { return throttle; }
float Car::GetBrake() { return brake; }
int Car::GetCurrentGear() { return current_gear; }
int Car::GetHighestGear() { return (int)gear_ratios.size(); }
float Car::GetRPMLimit() { return rpm_limit; }
