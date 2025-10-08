#pragma once
#include <GameState.hpp>
#include <raylib.h>
#include <cmath>
extern "C" {
    #include <raylib.h>
}

struct Projectile {
    Vector2 pos;
    Vector2 vel;
    bool active;
};

class MainGameState : public GameState
{
    public:
        MainGameState();
        ~MainGameState() = default;

        void init() override;
        void handleInput() override;
        void update(float deltaTime) override;
        void render() override;

        void pause(){};
        void resume(){};

    
    private:
        
        // Ventana
        const int screenWidth = 800;
        const int screenHeight = 600;

        // --- Jugador (cuadrado) ---
        Rectangle player1;
        
        // --- Enemigo (triángulo) ---
        Vector2 trianglePos;
        float triangleSize = 50;

        // --- Suelo ---
        const int groundHeight = 100;
        Rectangle ground;

        // --- Proyectil ---
        Projectile projectile = {{0, 0}, {0, 0}, false};
        const float gravity = 400.0f;

        // --- Apuntado ---
        float angle = -30.0f * DEG2RAD; // ángulo inicial (en radianes)
        const float arrowLength = 60.0f;
        const float angleSpeed = 60.0f * DEG2RAD; // velocidad de rotación en rad/s
};