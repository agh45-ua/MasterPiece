#include "StateMachine.hpp"
#include "InicioState.hpp"
#include "PersonajesState.hpp"
#include <memory>
#include <chrono>
#include <libintl.h>
#include <locale.h>
extern "C" {
    #include <raylib.h>
}

int main()
{
    // Configurar gettext para internacionalización
    setlocale(LC_ALL, "");
    bindtextdomain("myapp", "./locale");
    textdomain("myapp");
    const int screenWidth = 800;
    const int screenHeight = 600;

    // --- Inicializar Raylib ---
    InitWindow(screenWidth, screenHeight, "Master Piece Game - Cabras Absolutas");
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);

    // --- Máquina de estados ---
    StateMachine state_machine;
    float delta_time = 0.0f;
    state_machine.add_state(std::make_unique<InicioState>(), false);
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
