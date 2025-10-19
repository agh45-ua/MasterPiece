#include "InicioState.hpp"
#include "StateMachine.hpp"
#include "GameState.hpp"
#include <iostream>
#include <string>
extern "C" {
    #include <raylib.h>
}

using namespace std;

InicioState::InicioState(){

}

void InicioState::init(){
    
}

void InicioState::handleInput(){

}

void InicioState::update(float deltaTime){

}

void InicioState::render() {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    int longitudTitulo = MeasureText("Master Piece", 100);
    DrawText("Master Piece", (GetScreenWidth() - longitudTitulo) / 2 , 120, 100, BLACK);

    Rectangle btnPlay = { (float)(GetScreenWidth()/2 - 150), 280, 300, 60 };
    Rectangle btnExit = { (float)(GetScreenWidth()/2 - 150), 360, 300, 60 };

    // Hover (visual)
    Vector2 m = GetMousePosition();
    bool playHover = CheckCollisionPointRec(m, btnPlay);
    bool exitHover = CheckCollisionPointRec(m, btnExit);

    // Fondo botones
    DrawRectangleRounded(btnPlay, 0.2f, 8, playHover ? DARKGREEN : GREEN);
    DrawRectangleRounded(btnExit, 0.2f, 8, exitHover ? MAROON : RED);

    // Texto botones (centrado)
    const int fontBtn = 24;
    const char* tPlay = "Jugar";
    const char* tExit = "Salir";
    int wPlay = MeasureText(tPlay, fontBtn);
    int wExit = MeasureText(tExit, fontBtn);

    DrawText(tPlay,
             (int)(btnPlay.x + (btnPlay.width - wPlay) / 2),
             (int)(btnPlay.y + (btnPlay.height - fontBtn) / 2),
             fontBtn, RAYWHITE);

    DrawText(tExit,
             (int)(btnExit.x + (btnExit.width - wExit) / 2),
             (int)(btnExit.y + (btnExit.height - fontBtn) / 2),
             fontBtn, RAYWHITE);

    DrawText("© 2025", 10, GetScreenHeight() - 26, 20, GRAY);

    EndDrawing();
}


void InicioState::pause(){

}

void InicioState::resume(){
    
}