#include <cassert>
#include <iostream>
#include "states/MainGameState.hpp"
#include "core/StateMachine.hpp"

using namespace std;

int main() {
    Arma canon = {"Canon", 25, 100.0f, 10.0f, RED};
    Jugador j1 = {1, "P1", {}, canon};
    Jugador j2 = {2, "P2", {}, canon};

    MainGameState game(j1, j2);
    StateMachine sm;
    game.setStateMachine(&sm);
    game.setGround({0, 500, 800, 100});

    // --- TEST 11: Movimiento y Gravedad ---
    game.setTurno('r');
    game.setProjectile1({100.0f, 100.0f}, {100.0f, 0.0f});
    game.update(1.0f);
    assert(game.isProjectile1Active() == true);
    cout << "[OK] Test 11: Físicas y Gravedad" << endl;

    // --- TEST 12: Límites de pantalla ---
    game.setTurno('r');
    game.setProjectile1({850.0f, 100.0f}, {0.0f, 0.0f});
    game.update(0.01f);
    assert(game.isProjectile1Active() == false);
    cout << "[OK] Test 12: Límite lateral" << endl;

    // --- TEST 13: Colisión con el suelo ---
    game.setTurno('r');
    game.setProjectile1({100.0f, 501.0f}, {0.0f, 0.0f});
    game.update(0.01f);
    assert(game.isProjectile1Active() == false);
    cout << "[OK] Test 13: Colisión suelo" << endl;

    return 0;
}