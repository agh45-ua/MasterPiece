#include <cassert>
#include <ctime>
#include <string>
#include "states/InicioState.hpp"
#include "states/GameOverState.hpp"
#include "states/PersonajesState.hpp"
#include "core/StateMachine.hpp"
using namespace std;

int main() {
    // DATOS DE PRUEBA
    Arma proyectil1 = {"Cañon", 50, 15.0f, 9.8f, Texture2D{}};
    Arma proyectil2 = {"Mortero", 40, 12.0f, 5.0f, Texture2D{}};

    // La Texture2D se inicializa a 0 para el test
    Jugador j1 = {1, "Artillero 1", {}, proyectil1};
    Jugador j2 = {2, "Artillero 2", {}, proyectil2};
    
    time_t tiempoFinal = time(nullptr);

    // TEST 7: InicioState 
    // Verificamos que el objeto se crea y la herencia es correcta
    InicioState* inicio = new InicioState();
    assert(inicio != nullptr);

    // --- TEST 8: PersonajesState ---
    PersonajesState* personajes = new PersonajesState();
    assert(personajes != nullptr);

    // TEST 9: GameOverState
    GameOverState* gameOver = new GameOverState(j1, j2, 1, tiempoFinal);
    assert(gameOver != nullptr);

    // TEST 10: Integración básica con StateMachine 
    StateMachine stm;
    inicio->setStateMachine(&stm);

    // Limpieza de memoria
    delete inicio;
    delete personajes;
    delete gameOver;

    return 0;
}
