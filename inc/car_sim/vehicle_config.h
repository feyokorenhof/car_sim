#pragma once

#include <string>
#include <vector>
#include <yaml-cpp/yaml.h>

struct TorquePoint
{
    double rpm;
    double torque;
};

namespace YAML
{

template <> struct convert<TorquePoint>
{
    static Node encode(const TorquePoint &point)
    {
        Node node;
        node["rpm"] = point.rpm;
        node["torque"] = point.torque;
        return node;
    }

    static bool decode(const Node &node, TorquePoint &point)
    {
        if (!node.IsMap() || !node["rpm"] || !node["torque"])
        {
            return false;
        }

        point.rpm = node["rpm"].as<double>();
        point.torque = node["torque"].as<double>();

        return true;
    }
};

} // namespace YAML

struct VehicleConfig
{
    std::string name;
    double mass;
    double peak_torque;
    float drag;
    float frontal_area;
    float wheel_radius;
    float rolling_resistance;
    float final_drive;
    float gear_efficiency = 0.9;
    float rpm_limit = 6500.0f;
    float max_braking_force = 8000.f;
    std::vector<float> gear_ratios;
    std::vector<TorquePoint> torque_curve;
};

VehicleConfig load_vehicle_config(const std::string &path);
