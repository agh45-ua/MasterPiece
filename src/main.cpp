#include "StateMachine.hpp"
#include "MainGameState.hpp"
#include <memory>
#include <chrono>
extern "C" {
    #include <raylib.h>
}

int main()
{
    const int screenWidth = 800;
    const int screenHeight = 600;

    // --- Inicializar Raylib ---
    InitWindow(screenWidth, screenHeight, "Raylib - Apuntado con W/S y disparo con gravedad");
    SetTargetFPS(60);

    // --- Máquina de estados ---
    StateMachine state_machine;
    float delta_time = 0.0f;
    state_machine.add_state(std::make_unique<MainGameState>(), false);
    state_machine.handle_state_changes(delta_time);

    // --- Bucle principal ---
    while (!WindowShouldClose() && !state_machine.is_game_ending())
    {
        delta_time = GetFrameTime();

        state_machine.handle_state_changes(delta_time);
        state_machine.getCurrentState()->handleInput();
        state_machine.getCurrentState()->update(delta_time);
        state_machine.getCurrentState()->render();
    }

    // --- Cerrar ventana ---
    CloseWindow();
    return 0;
}
