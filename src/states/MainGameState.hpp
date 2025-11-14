#pragma once
#include "GameState.hpp"

#include <raylib.h>
#include <string>
#include <cmath>
extern "C" {
    #include <raylib.h>
#include "PersonajesState.hpp"
}

using namespace std;

struct Projectile {
    Vector2 pos;
    Vector2 vel;
    bool active;
    bool hasHit;
};

struct Player {
  Rectangle rect;
  int health = 100;
};

class MainGameState : public GameState
{
    public:
        MainGameState(const Jugador& p1, const Jugador& p2);
        ~MainGameState() = default;

        void init() override;
        void handleInput() override;
        void update(float deltaTime) override;
        void render() override;

        void pause(){};
        void resume(){};

    
    private:

        Jugador jugador1;
        Jugador jugador2;
        Texture2D fondo;

        // Ventana
        const int screenWidth = 800;
        const int screenHeight = 600;

        // --- Jugador 1 y 2 (cuadrado) ---
        Player player1, old_player1;
        Player player2, old_player2;

        // --- Suelo ---
        const int groundHeight = 100;
        Rectangle ground;

        // --- Turnos ---
        char turno;
        int contador_turno =1;

        // --- Proyectiles ---
        Projectile projectile_1 = {{0, 0}, {0, 0}, false};
        const float gravity_1 = 400.0f;
        Projectile projectile_2 = {{0, 0}, {0, 0}, false};
        const float gravity_2 = 400.0f;

        // --- Apuntado ---
        float angle_1 = -30.0f * DEG2RAD, old_angle_1; // ángulo inicial (en radianes)
        const float arrowLength_1 = 60.0f;
        const float angleSpeed_1 = 60.0f * DEG2RAD; // velocidad de rotación en rad/s

        float angle_2 = -30.0f * DEG2RAD, old_angle_2; // ángulo inicial (en radianes)
        const float arrowLength_2 = 60.0f;
        const float angleSpeed_2 = 60.0f * DEG2RAD; // velocidad de rotación en rad/s


        //cuenta atrás
        bool countdownActive = false;   // si la cuenta atrás está en marcha
        float countdownTime = 3.0f;     // 3 segundos
        bool gameBlocked = false;       // bloquea todo mientras dura la cuenta atrás

        // Tiempo de juego
        time_t startTime;
};