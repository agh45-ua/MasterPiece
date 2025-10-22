#pragma once
extern "C" {
    #include <raylib.h>
}
#include <iostream>
#include "GameState.hpp"
#include "PersonajesState.hpp"

using namespace std;

class GameOverState: public GameState
{
    public:
        GameOverState(int winnerId);
        GameOverState(const Jugador& p1, const Jugador& p2);
        ~GameOverState() override = default;
        
        //Sobreescribimos los metodos virtuales
        void init() override;
        void handleInput() override;
        void update(float deltaTime) override;
        void render() override;
        void pause() override;
        void resume() override;
    
    private:
        int winner_id = 0;
        Jugador jugador1;
        Jugador jugador2;
        Texture2D fondo;
};
