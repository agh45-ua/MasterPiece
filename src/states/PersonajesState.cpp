#include "InicioState.hpp"
#include "MainGameState.hpp"
#include "PersonajesState.hpp"
#include "StateMachine.hpp"
#include "GameState.hpp"
#include <iostream>
#include <string>
#include "../core/config.h"
extern "C" {
    #include <raylib.h>
}

using namespace std;

PersonajesState::PersonajesState(){

}

void PersonajesState::init(){
    fondo = LoadTexture(GetAssetPath("fondo-juego.png").c_str());
    poppins = LoadFontEx(GetAssetPath("Poppins-Bold.ttf").c_str(), 120, 0, 0);
    color1 = 0;
    color2 = 1;
    confirmarJugador1 = false;
    confirmarJugador2 = false;
    colorConfirmar1 = GRAY;
    colorConfirmar2 = GRAY;

    for (int i = 0; i < 6; i++) {
        personajes[i] = LoadTexture(GetAssetPath(rutas[i]).c_str());
    }

    color1 = 0;
    color2 = 1;
    jugador1 = {1, "Jugador 1", personajes[color1]};
    jugador2 = {2, "Jugador 2", personajes[color2]};
}

void PersonajesState::handleInput(){

}

void PersonajesState::update(float deltaTime){
    Vector2 m = GetMousePosition();
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        int cuadrado = 100;
        int y = 300 - cuadrado/2;
        int x1 = 100;
        int x2 = 600;
        int centroY = y + cuadrado / 2;
        Rectangle j1_izq = { (float)(x1 - 60), (float)(centroY - 25), 50, 50 };
        Rectangle j1_der = { (float)(x1 + cuadrado + 10), (float)(centroY - 25), 50, 50 };
        Rectangle j2_izq = { (float)(x2 - 60), (float)(centroY - 25), 50, 50 };
        Rectangle j2_der = { (float)(x2 + cuadrado + 10), (float)(centroY - 25), 50, 50 };        
        const int N = 6;
        //Cambiar colores
        if(confirmarJugador1 == false){
            if (CheckCollisionPointRec(m, j1_izq)) {
                int c = (color1 - 1 + N) % N;
                if (c == color2) c = (c - 1 + N) % N;
                color1 = c;
                jugador1.personaje = personajes[color1];
            }
            if (CheckCollisionPointRec(m, j1_der)) {
                int c = (color1 + 1) % N;
                if (c == color2) c = (c + 1) % N;
                color1 = c;
                jugador1.personaje = personajes[color1];
            }
        }
        if(confirmarJugador2 == false){
            if (CheckCollisionPointRec(m, j2_izq)) {
                int c = (color2 - 1 + N) % N;
                if (c == color1) c = (c - 1 + N) % N;
                color2 = c;
                jugador2.personaje = personajes[color2];
            }
            if (CheckCollisionPointRec(m, j2_der)) {
                int c = (color2 + 1) % N;
                if (c == color1) c = (c + 1) % N;
                color2 = c;
                jugador2.personaje = personajes[color2];
            }
        }
        //Comprobar quien quiere cambiar de nombre
        if (CheckCollisionPointRec(m, Nombre1) && confirmarJugador1 == false){
            editActivo = 0;
        }
        else if (CheckCollisionPointRec(m, Nombre2) && confirmarJugador2 == false){
            editActivo = 1;
        }
        else{
            editActivo = -1;
        }

        //Confirmar jugadores
        Rectangle confirmar1 ={50,400,200,40};
        Rectangle confirmar2 ={550,400,200,40};
        if (CheckCollisionPointRec(m, confirmar1)) {
            if(confirmarJugador1 == false){
                colorConfirmar1 = GREEN;
                confirmarJugador1 = true;
            }
            else{
                colorConfirmar1 = GRAY;
                confirmarJugador1 = false;
            }
        }
        if (CheckCollisionPointRec(m, confirmar2)) {
            if(confirmarJugador2 == false){
                colorConfirmar2 = GREEN;
                confirmarJugador2 = true;
            }
            else{
                colorConfirmar2 = GRAY;
                confirmarJugador2 = false;
            }
        }

        //Empezar a jugar
        float anchoJugar = 200;
        float altoJugar = 60;
        float xJugar = (GetScreenWidth() - anchoJugar) / 2;
        float yJugar = (GetScreenHeight() - altoJugar) / 2;
        
        Rectangle jugar = { xJugar, yJugar, anchoJugar, altoJugar };
        if (CheckCollisionPointRec(m, jugar) && confirmarJugador1 == true && confirmarJugador2 == true) {
            this->state_machine->add_state(std::make_unique<MainGameState>(jugador1, jugador2), true);
        }

        // Botón Volver
        Rectangle volver = { 20, 20, 120, 40 };
        if (CheckCollisionPointRec(m, volver)) {
            this->state_machine->add_state(std::make_unique<InicioState>(), true);
            return;
        }

    }
    //Escribir nuevo nombre
    if (editActivo != -1) {
        int ch;
        while ((ch = GetCharPressed()) > 0) {
            if (ch >= 32 && ch <= 126) {
                if (editActivo == 0 && (int)jugador1.nombre.size() < maxNombre) {
                    jugador1.nombre.push_back((char)ch);
                } else if (editActivo == 1 && (int)jugador2.nombre.size() < maxNombre) {
                    jugador2.nombre.push_back((char)ch);
                }
            }
        }
        if (IsKeyPressed(KEY_BACKSPACE)) {
            if (editActivo == 0 && !jugador1.nombre.empty()) {
                jugador1.nombre.pop_back();
            } else if (editActivo == 1 && !jugador2.nombre.empty()) {
                jugador2.nombre.pop_back();
            }
        }
        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_ESCAPE)) {
            editActivo = -1;
        }
    }
    // Cuando los dos jugadores confirman, empieza la transición
    if (confirmarJugador1 && confirmarJugador2) {
        mostrandoJugar = true;
        if (transicionAlpha < 1.0f){
            transicionAlpha += deltaTime * 5.0f;
        }
    }
    else {
        mostrandoJugar = false;
        if (transicionAlpha > 0.0f){
            transicionAlpha -= deltaTime * 5.0f;
        }
    }
}

void PersonajesState::render() {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    //Dibujar Fondo
    DrawTexturePro(
        fondo,
        { 0, 0, (float)fondo.width, (float)fondo.height },
        { 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() },
        { 0, 0 },
        0.0f,
        WHITE
    );

    Vector2 mouse = GetMousePosition();

    //Variables Personajes
    int ancho = GetScreenWidth();
    int alto = GetScreenHeight();
    int cuadrado = 100;
    int y = 300 - cuadrado/2;
    int x1 = 100;
    int x2 = 600;

    DrawTexturePro(
        jugador1.personaje,
        { 0, 0, (float)jugador1.personaje.width, (float)jugador1.personaje.height },
        { (float)x1, (float)y, (float)cuadrado, (float)cuadrado },
        { 0, 0 },
        0.0f,
        WHITE
    );
    DrawTexturePro(
        jugador2.personaje,
        { 0, 0, (float)jugador2.personaje.width, (float)jugador2.personaje.height },
        { (float)x2, (float)y, (float)cuadrado, (float)cuadrado },
        { 0, 0 },
        0.0f,
        WHITE
    );

    //Variables Nombres
    int anchoCaja = 200;
    int altoCaja = 40;
    int sepY = 20;
    int cx1 = x1 + cuadrado/2;
    int cx2 = x2 + cuadrado/2;
    int yNombre = y - sepY - altoCaja;
    Nombre1 = { (float)(cx1 - anchoCaja/2), (float)yNombre, (float)anchoCaja, (float)altoCaja };
    Nombre2 = { (float)(cx2 - anchoCaja/2), (float)yNombre, (float)anchoCaja, (float)altoCaja };

    //Cuadrados Nombres
    DrawRectangleRec(Nombre1, LIGHTGRAY);
    DrawRectangleLinesEx(Nombre1, 2, (editActivo==0)? BLACK : DARKGRAY);
    DrawRectangleRec(Nombre2, LIGHTGRAY);
    DrawRectangleLinesEx(Nombre2, 2, (editActivo==1)? BLACK : DARKGRAY);

    int padX = 10, padY = 10;
    DrawText(jugador1.nombre.c_str(), (int)Nombre1.x + padX, (int)Nombre1.y + padY, 20, BLACK);
    DrawText(jugador2.nombre.c_str(), (int)Nombre2.x + padX, (int)Nombre2.y + padY, 20, BLACK);

    if (editActivo == 0) {
        int w = MeasureText(jugador1.nombre.c_str(), 20);
        if (fmod(GetTime(), 1.0) < 0.5) DrawRectangle((int)Nombre1.x + padX + w + 2, (int)Nombre1.y + padY, 10, 20, BLACK);
    }
    if (editActivo == 1) {
        int w = MeasureText(jugador2.nombre.c_str(), 20);
        if (fmod(GetTime(), 1.0) < 0.5) DrawRectangle((int)Nombre2.x + padX + w + 2, (int)Nombre2.y + padY, 10, 20, BLACK);
    }
    // Flechas jugador 1
    int centroY1 = y + cuadrado / 2;

    //Flecha izquierda jugador 1
    Vector2 j1_izqA = { (float)(x1 - 40), (float)centroY1 };
    Vector2 j1_izqB = { (float)(x1 - 10), (float)(centroY1 + 20) };
    Vector2 j1_izqC = { (float)(x1 - 10), (float)(centroY1 - 20) };
    Rectangle j1_izq = { (float)(x1 - 60), (float)(centroY1 - 25), 50, 50 };
    bool hover_j1_izq = CheckCollisionPointRec(mouse, j1_izq);
    Color color_j1_izq = hover_j1_izq && confirmarJugador1 == false ? DARKGRAY : BLACK;
    DrawTriangle(j1_izqA, j1_izqB, j1_izqC, color_j1_izq);

    //Flecha derecha jugador 1
    Vector2 j1_derA = { (float)(x1 + cuadrado + 40), (float)centroY1 };
    Vector2 j1_derB = { (float)(x1 + cuadrado + 10), (float)(centroY1 - 20) };
    Vector2 j1_derC = { (float)(x1 + cuadrado + 10), (float)(centroY1 + 20) };
    Rectangle j1_der = { (float)(x1 + cuadrado + 10), (float)(centroY1 - 25), 50, 50 };
    bool hover_j1_der = CheckCollisionPointRec(mouse, j1_der);
    Color color_j1_der = hover_j1_der && confirmarJugador1 == false ? DARKGRAY : BLACK;
    DrawTriangle(j1_derA, j1_derB, j1_derC, color_j1_der);

    // Flechas jugador 2 (izquierda y derecha del cuadrado)
    int centroY2 = y + cuadrado / 2;

    //Flecha izquierda jugador 2
    Vector2 j2_izqA = { (float)(x2 - 40), (float)centroY2 };
    Vector2 j2_izqB = { (float)(x2 - 10), (float)(centroY2 + 20) };
    Vector2 j2_izqC = { (float)(x2 - 10), (float)(centroY2 - 20) };
    Rectangle j2_izq = { (float)(x2 - 60), (float)(centroY1 - 25), 50, 50 };
    bool hover_j2_izq = CheckCollisionPointRec(mouse, j2_izq);
    Color color_j2_izq = hover_j2_izq && confirmarJugador2 == false ? DARKGRAY : BLACK;
    DrawTriangle(j2_izqA, j2_izqB, j2_izqC, color_j2_izq);

    //Flecha derecha jugador 2
    Vector2 j2_derA = { (float)(x2 + cuadrado + 40), (float)centroY2 };
    Vector2 j2_derB = { (float)(x2 + cuadrado + 10), (float)(centroY2 - 20) };
    Vector2 j2_derC = { (float)(x2 + cuadrado + 10), (float)(centroY2 + 20) };
    Rectangle j2_der = { (float)(x2 + cuadrado + 10), (float)(centroY1 - 25), 50, 50 };
    bool hover_j2_der = CheckCollisionPointRec(mouse, j2_der);
    Color color_j2_der = hover_j2_der && confirmarJugador2 == false ? DARKGRAY : BLACK;
    DrawTriangle(j2_derA, j2_derB, j2_derC, color_j2_der);

    //Botones de confirmar
    float anchoConfirmar = 200;
    float altoConfirmar = 40;
    float yConfirmar = 400;
    float x1Confirmar = 50;
    float x2Confirmar = 550;
    
    Rectangle confirmar1 ={x1Confirmar,yConfirmar,anchoConfirmar,altoConfirmar};
    Rectangle confirmar2 ={x2Confirmar,yConfirmar,anchoConfirmar,altoConfirmar};

    DrawRectangleRounded(confirmar1, 0.3f, 8, colorConfirmar1);
    DrawRectangleRounded(confirmar2, 0.3f, 8, colorConfirmar2);

    //Texto de confirmar
    float fuenteConfirmar = 24.0f;
    float espacioConfirmar = 2.0f;
    Vector2 longitudConfirmar = MeasureTextEx(poppins, "Confirmar", fuenteConfirmar, espacioConfirmar);
    float textX = x1Confirmar + (anchoConfirmar - longitudConfirmar.x) / 2;
    float textY = yConfirmar + (altoConfirmar - longitudConfirmar.y) / 2;
    DrawTextEx(poppins, "Confirmar", { textX, textY }, fuenteConfirmar, espacioConfirmar, RAYWHITE);
    float textX2 = x2Confirmar + (anchoConfirmar - longitudConfirmar.x) / 2;
    float textY2 = yConfirmar + (altoConfirmar - longitudConfirmar.y) / 2;
    DrawTextEx(poppins, "Confirmar", { textX2, textY2 }, fuenteConfirmar, espacioConfirmar, RAYWHITE);

    //Boton de Jugar
    if(transicionAlpha > 0.0f){
        float anchoJugar = 200;
        float altoJugar = 60;
        float xJugar = (GetScreenWidth() - anchoJugar) / 2;
        float yJugar = (GetScreenHeight() - altoJugar) / 2;

        Rectangle jugar = { xJugar, yJugar, anchoJugar, altoJugar };

        bool hover = CheckCollisionPointRec(mouse, jugar);
        Color colorJugar = hover ? GREEN : GRAY;
        Color transicion = Fade(colorJugar, transicionAlpha);


        DrawRectangleRounded(jugar, 0.3f, 8, transicion);

        //Texto de jugar
        float fuenteJugar = 35.0f;
        float espacioJugar = 2.0f;
        Vector2 longitudJugar = MeasureTextEx(poppins, "Jugar", fuenteJugar, espacioJugar);
        float textXJugar = xJugar + (anchoJugar - longitudJugar.x) / 2;
        float textYJugar = yJugar + (altoJugar - longitudJugar.y) / 2;
        DrawTextEx(poppins, "Jugar", { textXJugar, textYJugar }, fuenteJugar, espacioJugar, Fade(RAYWHITE, transicionAlpha));
    }
    // Botón Volver
    float anchoVolver = 120;
    float altoVolver = 40;
    float xVolver = 20;
    float yVolver = 20;

    Rectangle volver = { xVolver, yVolver, anchoVolver, altoVolver };
    bool hoverVolver = CheckCollisionPointRec(mouse, volver);
    Color colorVolver = hoverVolver ? MAROON : RED;

    DrawRectangleRounded(volver, 0.3f, 8, colorVolver);

    float fuenteVolver = 22.0f;
    float espacioVolver = 2.0f;
    Vector2 textoVolver = MeasureTextEx(poppins, "Volver", fuenteVolver, espacioVolver);
    float textXVolver = xVolver + (anchoVolver - textoVolver.x) / 2;
    float textYVolver = yVolver + (altoVolver - textoVolver.y) / 2;
    DrawTextEx(poppins, "Volver", { textXVolver, textYVolver }, fuenteVolver, espacioVolver, RAYWHITE);

    EndDrawing();
}



void PersonajesState::pause(){

}

void PersonajesState::resume(){
    
}
