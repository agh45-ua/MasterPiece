#pragma once
#include "GameState.hpp"
#include "config.h"
#include <cmath>
#include <string>
#include <vector>
extern "C" {
    #include <raylib.h>
}

using namespace std;

struct Arma{
    string nombre;
    int damage;
    float speed;
    float gravity;
    Color colorProyectil;
};

struct Jugador{
    int id;
    string nombre;
    Texture2D personaje;
    Arma arma;
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
        string rutas[6] = {"Personaje1.png", "Personaje2.png", "Personaje3.png", "Personaje4.png", "Personaje5.png", "Personaje6.png"};
        
        int color1;
        int color2;
        
        //Variables para la selección de arma
        vector<Arma> armasDisponibles;
        int indiceArma1 = 0;
        int indiceArma2 = 0;

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
