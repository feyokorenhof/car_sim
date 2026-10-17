#include "../inc/car_sim/vehicle_config.h"
#include <yaml-cpp/yaml.h>

VehicleConfig load_vehicle_config(const std::string &path)
{
    YAML::Node config = YAML::LoadFile(path);
    return VehicleConfig{
        .name = config["vehicle"]["name"].as<std::string>(),
        .mass = config["vehicle"]["mass"].as<float>(),
        .peak_torque = config["engine"]["peak_torque"].as<float>(),
        .drag = config["vehicle"]["drag"].as<float>(),
        .frontal_area = config["vehicle"]["frontal_area"].as<float>(),
        .wheel_radius = config["wheels"]["wheel_radius"].as<float>(),
        .rolling_resistance =
            config["vehicle"]["rolling_resistance"].as<float>(),
        .final_drive = config["transmission"]["final_drive"].as<float>(),
        .gear_efficiency =
            config["transmission"]["gear_efficiency"].as<float>(),
        .rpm_limit = config["engine"]["rpm_limit"].as<float>(),
        .max_braking_force = config["wheels"]["max_braking_force"].as<float>(),
        .gear_ratios =
            config["transmission"]["gear_ratios"].as<std::vector<float>>(),
        .torque_curve =
            config["engine"]["torque_curve"].as<std::vector<TorquePoint>>()};
}
