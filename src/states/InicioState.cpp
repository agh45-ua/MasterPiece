#include "InicioState.hpp"
#include "PersonajesState.hpp"
#include "MainGameState.hpp"
#include "StateMachine.hpp"
#include "GameState.hpp"
#include <iostream>
#include <string>
#include "../core/config.h"
extern "C" {
    #include <raylib.h>
}

using namespace std;

InicioState::InicioState(){

}

void InicioState::init(){
    fondo = LoadTexture(GetAssetPath("fondo-inicio.png").c_str());
    poppins = LoadFontEx(GetAssetPath("Poppins-Bold.ttf").c_str(), 120, 0, 0);
}

void InicioState::handleInput(){

}

void InicioState::update(float deltaTime){
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Vector2 m = GetMousePosition();
        Rectangle Jugar = { (float)(GetScreenWidth()/2 - 150), 280, 300, 60 };
        Rectangle Salir = { (float)(GetScreenWidth()/2 - 150), 360, 300, 60 };
        if (CheckCollisionPointRec(m, Jugar)) {
            this->state_machine->add_state(make_unique<PersonajesState>(), true);
        }
        if (CheckCollisionPointRec(m, Salir)) {
            CloseWindow();
            exit(0);
        }
    }

}

void InicioState::render() {
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
    Rectangle Jugar = { (float)(GetScreenWidth()/2 - 150), 280, 300, 60 };
    Rectangle Salir = { (float)(GetScreenWidth()/2 - 150), 360, 300, 60 };
    Vector2 m = GetMousePosition();
    bool HoverJugar = CheckCollisionPointRec(m, Jugar);
    bool HoverSalir = CheckCollisionPointRec(m, Salir);
    DrawRectangleRounded(Jugar, 0.2f, 8, HoverJugar ? DARKGREEN : GREEN);
    DrawRectangleRounded(Salir, 0.2f, 8, HoverSalir ? MAROON : RED);

    const int fuente = 24;
    int anchoJugar = MeasureTextEx(poppins, "Jugar", fuente, 4).x;
    int anchoSalir = MeasureTextEx(poppins, "Salir", fuente, 4).x;

    DrawTextEx(poppins, "Jugar",{ (float)(Jugar.x + (Jugar.width - anchoJugar) / 2),(float)(Jugar.y + (Jugar.height - fuente) / 2) },(float)fuente, 2, RAYWHITE);
    DrawTextEx(poppins, "Salir",{ (float)(Salir.x + (Salir.width - anchoSalir) / 2),(float)(Salir.y + (Salir.height - fuente) / 2) },(float)fuente, 2, RAYWHITE);

    EndDrawing();
}


void InicioState::pause(){

}

void InicioState::resume(){
    
}
