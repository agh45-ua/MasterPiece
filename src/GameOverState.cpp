extern "C" {
    #include <raylib.h>
}
#include "StateMachine.hpp"
#include "MainGameState.hpp"
#include "GameOverState.hpp"
#include "GameState.hpp"
#include <iostream>

using namespace std;

GameOverState::GameOverState(const Jugador& a, const Jugador& b)
: jugador1(a), jugador2(b) {}

void GameOverState::init(){
    fondo = LoadTexture("assets/fondo-juego.png");    
}

void GameOverState::handleInput(){
    if(IsKeyPressed(KEY_SPACE)){
        this->state_machine->add_state(std::make_unique<MainGameState>(jugador1, jugador2), true);
    }
}

void GameOverState::update(float deltaTime){

}

void GameOverState::render(){
    
    BeginDrawing();

    ClearBackground(RAYWHITE);
    
    DrawTexturePro(
        fondo,
        { 0, 0, (float)fondo.width, (float)fondo.height },
        { 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() },
        { 0, 0 },
        0.0f,
        WHITE
    );

    DrawText("Game Over", 300, 300, 30, RED);

    EndDrawing();
}

void GameOverState::pause(){

}

void GameOverState::resume(){
    
}