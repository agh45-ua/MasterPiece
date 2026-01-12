#pragma once
#include "GameState.hpp"
#include "PersonajesState.hpp"
#include "config.h"
#include <string>
#include <cmath>
#include <ctime>
extern "C" {
    #include <raylib.h>
}

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

        //Metodos para los tests
        int getPlayer1Health() const { return player1.health; }
        int getPlayer2Health() const { return player2.health; }
        bool isProjectile1Active() const { return projectile_1.active; }
        void setTurno(char t) { turno = t; } // Para forzar la fase de resolución en el test
        void setProjectile1(Vector2 pos, Vector2 vel) {
            projectile_1.pos = pos;
            projectile_1.vel = vel;
            projectile_1.active = true;
        }
        void setGround(Rectangle g) { ground = g; }
    
    private:

        Jugador jugador1;
        Jugador jugador2;
        Texture2D fondo;
        Font poppins;

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
        Projectile projectile_1 = {{0, 0}, {0, 0}, false, false};
        Projectile projectile_2 = {{0, 0}, {0, 0}, false, false};

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