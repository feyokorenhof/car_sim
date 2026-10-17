#include "../inc/car_sim/car_game_3D.h"
#include "../inc/car_sim/vehicle_config.h"

int main()
{
    // VehicleConfig config =
    //     load_vehicle_config("assets/vehicles/ford_fiesta_2007_125i.yaml");
    // VehicleConfig config =
    //     load_vehicle_config("assets/vehicles/nissan_gt_r_r35_2025.yaml");
    // VehicleConfig config =
    //     load_vehicle_config("assets/vehicles/mclaren_p1.yaml");
    // VehicleConfig config =
    //     load_vehicle_config("assets/vehicles/bugatti_chiron.yaml");
    VehicleConfig config =
        load_vehicle_config("assets/vehicles/speed_demon.yaml");

    CarGame3D game = CarGame3D(1.0f / 120.0f, config);

    return 0;
}
