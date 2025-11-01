#pragma once
#include "GameState.hpp"
#include <raylib.h>
#include <cmath>
#include <string>
extern "C" {
    #include <raylib.h>
}

using namespace std;

struct Jugador{
    int id;
    string nombre;
    Texture2D personaje;
};

class PersonajesState: public GameState {
    public:
        PersonajesState();
        ~PersonajesState() = default;

        void init() override;
        void handleInput() override;
        void update(float deltaTime) override;
        void render() override;

        void pause();
        void resume();
    private:
        Texture2D fondo;
        Font poppins;
        Jugador jugador1;
        Jugador jugador2;
        Texture2D personajes[6];
        string rutas[6] = {"assets/Personaje1.png", "assets/Personaje2.png", "assets/Personaje3.png", "assets/Personaje4.png", "assets/Personaje5.png", "assets/Personaje6.png"};
        int color1;
        int color2;
        Rectangle Nombre1{}, Nombre2{};
        int editActivo = -1;
        int maxNombre = 14;
        bool confirmarJugador1;
        bool confirmarJugador2;
        Color colorConfirmar1;
        Color colorConfirmar2;
        float transicionAlpha = 0.0f;
        bool mostrandoJugar = false;
};