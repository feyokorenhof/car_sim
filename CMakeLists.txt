
cmake_minimum_required(VERSION 3.24)
project(CarSim LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(yaml-cpp CONFIG REQUIRED)
find_package(raylib REQUIRED)

add_executable(CarSim
    src/main.cpp
    src/car.cpp
    src/vehicle_config.cpp
)


target_link_libraries(CarSim PRIVATE yaml-cpp::yaml-cpp)
target_link_libraries(CarSim PRIVATE raylib)

target_compile_definitions(CarSim
    PRIVATE
        CAR_SIM_ASSET_DIR="${CMAKE_CURRENT_SOURCE_DIR}/assets"
)
