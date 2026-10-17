#include "./car.h"
#include "./game.h"
#include "vehicle_config.h"
#include <cstdio>
#include <unistd.h>

class CarGame : public Game
{
  private:
    Car *my_car;

  public:
    CarGame(float fixedDt) : Game(fixedDt) {}
    void Start(bool terminalBased = false) override
    {

        VehicleConfig config =
            load_vehicle_config("assets/vehicles/porsche_911.yaml");
        my_car = new Car(config);

        Game::Start(true);
    }

    void Update(float deltaTime) override
    {
        RenderUI(deltaTime);
        char c = 0;
        if (read(STDIN_FILENO, &c, 1) == 1)
        {
            if (c == 'w' || c == 'W')
            {
                my_car->SetThrottle(1.0f);
                my_car->SetBrake(0.0f);
            }
            else if (c == 's' || c == 'S')
            {
                my_car->SetBrake(1.0f);
                my_car->SetThrottle(0.0f);
            }
            else
            {
                float current_t = my_car->GetThrottle();
                float current_b = my_car->GetBrake();
                my_car->SetThrottle(
                    std::max(0.0f, current_t - 2.0f * deltaTime));
                my_car->SetBrake(std::max(0.0f, current_b - 2.0f * deltaTime));
            }

            // Shift
            if (c == 'e' || c == 'E')
            {
                my_car->Shift(1);
            }
            else if (c == 'q' || c == 'Q')
            {
                my_car->Shift(-1);
            }
        }
    }

    void PhysicsUpdate() override { my_car->Accelerate(FIXED_DT); }

  private:
    void RenderUI(float deltaTime)
    {
        printf("\033[H");
        printf("%s\n", my_car->GetName().c_str());
        printf("SNELHEID: %f km/u\n", my_car->GetVelocityKmH());
        printf("VERSNELLING: %i / %i\n", my_car->GetCurrentGear(),
               my_car->GetHighestGear());
        float rpm_pct = my_car->GetRPM() / my_car->GetRPMLimit();
        int max_lines = 20;
        int lines = rpm_pct * max_lines;
        printf("[");
        for (int i = 0; i < max_lines; ++i)
        {
            if (i < lines)
            {
                printf("|");
            }
            else
            {
                printf(" ");
            }
        }
        printf("]\n");
    }
};
