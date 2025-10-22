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
    Color color;
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
        Color colores[6] = {RED, GREEN, SKYBLUE, YELLOW, ORANGE, PINK};
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