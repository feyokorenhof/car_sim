#include "vehicle_config.h"
#include <stdio.h>
#include <string>

struct Car
{
  private:
    // Const vars
    const std::string name;
    const float mass;
    const float peak_torque;
    const float drag;
    const float frontal_area;

    const float wheel_radius;
    const float rolling_resistance;
    const float final_drive;
    const float gear_efficiency = 0.9;
    const float rpm_limit = 6500.0f;
    const float max_braking_force = 8000.f;
    const std::vector<float> gear_ratios;
    const std::vector<TorquePoint> torque_curve;

    // Dynamic vars
    float current_hp;
    float current_torque;

    float velocity = 0.0f;

    int current_gear = 1;

    float throttle = 0.f;
    float brake = 0.f;

  public:
    Car(VehicleConfig config);
    Car();

    std::string GetName();
    int GetPeakHp();

    // Wheel speed in rad/s
    float GetWheelSpeed();
    float GetTorque();

    float GetRPM();

    // In Newtons
    float GetDriveForce();

    float GetAeroDrag();

    float GetRollingResistance();

    float GetNettoForce();

    float GetAcceleration();

    void Accelerate(const float FIXED_DT);

    void SetThrottle(float t);
    void SetBrake(float b);
    void Shift(int dir);

    float GetVelocityKmH();
    float GetThrottle();
    float GetBrake();
    int GetCurrentGear();
    int GetHighestGear();
    float GetRPMLimit();
};
