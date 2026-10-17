#include <chrono>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

class Game
{
  private:
    struct termios orig_termios;
    float accumulator = 0.f;
    std::chrono::high_resolution_clock clock;
    std::chrono::high_resolution_clock::time_point previous_frame_time;

    bool terminalBased = false;
    bool canRun = false;

    void PrepareTerminalMacOS()
    {
        // Terminal stuff (macOS)
        // Save original term settings
        tcgetattr(STDIN_FILENO, &orig_termios);

        struct termios raw = orig_termios;
        // Turn off ICANON (wait for enter) and ECHO
        raw.c_lflag &= ~(ICANON | ECHO);

        // Non-blocking
        raw.c_cc[VMIN] = 0;
        raw.c_cc[VTIME] = 0;

        // Apply
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);

        // Clear term
        printf("\033[?25l");
        printf("\033[2J");
    }

  protected:
    const float FIXED_DT = 1.0 / 120.0f;

  public:
    Game(const float fixed_dt) : FIXED_DT(fixed_dt) {}
    virtual ~Game()
    {
        if (terminalBased)
            tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
    }

    virtual void Start(bool terminalBased = false)
    {
        terminalBased = terminalBased;
        if (terminalBased)
        {
            PrepareTerminalMacOS();
        }

        previous_frame_time = clock.now();
        canRun = true;

        while (canRun)
        {
            auto now = clock.now();
            if (previous_frame_time == now)
                continue;
            float frameTime = std::clamp(
                std::chrono::duration<float>(now - previous_frame_time).count(),
                0.f, 0.1f);

            previous_frame_time = now;
            Update(frameTime);
            accumulator += frameTime;
            while (accumulator >= FIXED_DT)
            {
                PhysicsUpdate();
                accumulator -= FIXED_DT;
            }
            Draw();
        }
    }

    virtual void Stop() { canRun = false; }

    virtual void Update(float deltaTime) {}

    virtual void PhysicsUpdate() {}

    virtual void Draw() {}
};
