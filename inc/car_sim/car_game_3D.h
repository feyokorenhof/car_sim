#pragma once

#include "../car_sim/car.h"
#include "../car_sim/game.h"
#include "vehicle_config.h"
#include <raylib.h>

class CarGame3D : public Game
{
  private:
    float carZ = 0.0f;
    Car car;
    Camera3D camera{};
    float roadOffset = 0.0f;
    const float dashSpacing = 8.0f;
    const int dashCount = 30;

  public:
    CarGame3D(const float FIXED_DT, VehicleConfig config)
        : Game(FIXED_DT), car(config)
    {
        InitWindow(1280, 720, "Car Simulator");
        SetTargetFPS(144);

        camera.position = {0.0f, 4.0f, -8.0f};
        camera.target = {0.0f, 0.0f, 3.0f};
        camera.up = {0.0f, 1.0f, 0.0f};
        camera.fovy = 60.0f;
        camera.projection = CAMERA_PERSPECTIVE;

        Game::Start(false);
    }

    ~CarGame3D() {}

    void Draw() override
    {
        BeginDrawing();
        ClearBackground(SKYBLUE);

        BeginMode3D(camera);

        for (int i = 0; i < dashCount; i++)
        {
            float z = camera.target.z + i * dashSpacing - roadOffset;

            DrawCube({0.0f, 0.01f, z}, 0.15f, 0.02f, 3.0f, WHITE);
        }

        // Green ground
        DrawPlane({0.0f, -0.1f, carZ + 20.0f}, {200.0f, 200.0f}, DARKGREEN);

        // Gray road
        DrawPlane({0.0f, -0.05f, carZ + 20.0f}, {12.0f, 200.0f}, DARKGRAY);

        // Temporary car model: a simple box.
        DrawCube({0.0f, 0.5f, carZ}, 2.0f, 1.0f, 4.0f, RED);

        DrawCubeWires({0.0f, 0.5f, carZ}, 2.0f, 1.0f, 4.0f, MAROON);

        EndMode3D();

        // HUD.
        DrawText(car.GetName().c_str(), 20, 20, 28, WHITE);
        DrawText(TextFormat("Speed: %.1f km/h", car.GetVelocityKmH()), 20, 60,
                 24, WHITE);
        DrawText(TextFormat("Gear: %d / %d", car.GetCurrentGear(),
                            car.GetHighestGear()),
                 20, 92, 24, WHITE);
        DrawText(TextFormat("RPM: %.0f", car.GetRPM()), 20, 124, 24, WHITE);

        float rpmRatio =
            std::clamp(car.GetRPM() / car.GetRPMLimit(), 0.0f, 1.0f);

        DrawRectangle(20, 165, 240, 20, DARKGRAY);
        DrawRectangle(20, 165, (int)(240 * rpmRatio), 20, ORANGE);
        DrawRectangleLines(20, 165, 240, 20, WHITE);

        DrawText("W: Accelerate  S: Brake  Q/E: Shift", 20, 205, 18, WHITE);

        EndDrawing();
    }

    void Update(float deltaTime) override
    {
        // Input: W accelerates, S brakes.
        car.SetThrottle(IsKeyDown(KEY_W) ? 1.0f : 0.0f);
        car.SetBrake(IsKeyDown(KEY_S) ? 1.0f : 0.0f);

        if (IsKeyPressed(KEY_E))
            car.Shift(1);

        if (IsKeyPressed(KEY_Q))
            car.Shift(-1);

        if (WindowShouldClose() || IsKeyPressed(KEY_ESCAPE))
            Stop();

        // Velocity is in m/s; FIXED_DT is physics timestep
        if (roadOffset >= dashSpacing)
            roadOffset -= dashSpacing;

        // Follow the car from behind.
        camera.position = {0.0f, 4.0f, carZ - 8.0f};
        camera.target = {0.0f, 0.5f, carZ + 3.0f};
    }
    void PhysicsUpdate() override
    {
        car.Accelerate(FIXED_DT);

        roadOffset += car.GetVelocityKmH() / 3.6f * FIXED_DT;
    }
};
