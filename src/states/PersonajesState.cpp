#include "InicioState.hpp"
#include "MainGameState.hpp"
#include "PersonajesState.hpp"
#include "StateMachine.hpp"
#include "GameState.hpp"
#include "ResourceManager.hpp"
#include <iostream>
#include <string>
#include <cstdlib>
#include "../core/config.h"
extern "C" {
    #include <raylib.h>
}

using namespace std;

PersonajesState::PersonajesState(){

}

void PersonajesState::init(){
    // CARGAR TEXTURAS
    fondo = ResourceManager::getInstance().GetTexture(GetAssetPath("fondo-juego.png"));
    poppins = ResourceManager::getInstance().GetFont(GetAssetPath("Poppins-Bold.ttf"));

    // INICIALIZAMOS VARIABLES
    color1 = 0;
    color2 = 1;
    confirmarJugador1 = false;
    confirmarJugador2 = false;
    colorConfirmar1 = RED;
    colorConfirmar2 = RED;

    // TEXTURAS PERSONAJES
    for (int i = 0; i < 6; i++) {
        personajes[i] = ResourceManager::getInstance().GetTexture(GetAssetPath(rutas[i]));
    }

    // TEXTURAS ARMAS
    lanzacohetes = LoadTexture(GetAssetPath("armas/lanzacohetes.png").c_str());
    pistola = LoadTexture(GetAssetPath("armas/pistola.png").c_str());
    sniper = LoadTexture(GetAssetPath("armas/sniper.png").c_str());

    // DEFINICIÓN DE ARMAS
    armasDisponibles = {
        Arma{"Pistola", 35, 650.0f, 600.0f, pistola},
        Arma{"Sniper", 20, 900.0f, 250.0f, sniper},       // Rápida, poca caída, menos daño
        Arma{"Lanzacohetes", 50, 500.0f, 500.0f, lanzacohetes}, // Mucho daño, mucha gravedad
    };

    indiceArma1 = 0;
    indiceArma2 = 0;

    color1 = 0;
    color2 = 1;

    jugador1 = {1, _("Jugador 1"), personajes[color1], armasDisponibles[indiceArma1]};
    jugador2 = {2, _("Jugador 2"), personajes[color2], armasDisponibles[indiceArma2]};
}

void PersonajesState::handleInput(){

}

void PersonajesState::update(float deltaTime){
    Vector2 m = GetMousePosition();

    // DEFINICIÓN DE POSICIONES
    int cuadrado = 100;
    int y = 200;
    int x1 = 100;
    int x2 = 600;
    int centroY = y + cuadrado / 2;

    // Hitbox Flechas Personaje (Skin)
    Rectangle j1_izq = { (float)(x1 - 45), (float)(centroY - 20), 40, 40 };
    Rectangle j1_der = { (float)(x1 + cuadrado + 5), (float)(centroY - 20), 40, 40 };
    Rectangle j2_izq = { (float)(x2 - 45), (float)(centroY - 20), 40, 40 };
    Rectangle j2_der = { (float)(x2 + cuadrado + 5), (float)(centroY - 20), 40, 40 }; 

    // Hitbox armas
    float armaAreaWidth = 220.0f;
    float armaAreaY = y + cuadrado + 40;

    // Jugador 1
    Texture2D armaTex1 = jugador1.arma.sprite;
    float escala1 = armaAreaWidth / (float)armaTex1.width;
    escala1 *= 1.05f;
    float anchoFinal1 = armaTex1.width * escala1;
    float altoFinal1  = armaTex1.height * escala1;
    float xFinal1 = x1 + (cuadrado - anchoFinal1) / 2;
    float yFinal1 = armaAreaY - 20;
    float armaCentroY1 = yFinal1 + altoFinal1 / 2;

    float flechaOffsetX = 100.0f;
    Rectangle j1_arma_izq = { (float)(x1 - flechaOffsetX - 20), (float)(armaCentroY1 - 20), 40, 40 };
    Rectangle j1_arma_der = { (float)(x1 + cuadrado + flechaOffsetX - 20), (float)(armaCentroY1 - 20), 40, 40 };

    // Jugador 2
    Texture2D armaTex2 = jugador2.arma.sprite;
    float escala2 = armaAreaWidth / (float)armaTex2.width;
    escala2 *= 1.05f;
    float anchoFinal2 = armaTex2.width * escala2;
    float altoFinal2  = armaTex2.height * escala2;
    float xFinal2 = x2 + (cuadrado - anchoFinal2) / 2;
    float yFinal2 = armaAreaY - 20;
    float armaCentroY2 = yFinal2 + altoFinal2 / 2;

    Rectangle j2_arma_izq = { (float)(x2 - flechaOffsetX - 20), (float)(armaCentroY2 - 20), 40, 40 };
    Rectangle j2_arma_der = { (float)(x2 + cuadrado + flechaOffsetX - 20), (float)(armaCentroY2 - 20), 40, 40 };

    // Rectángulos Nombres
    int anchoCaja = 200; int altoCaja = 40; int sepY = 20;
    int cx1 = x1 + cuadrado/2; int cx2 = x2 + cuadrado/2;
    int yNombre = y - sepY - altoCaja;
    
    Nombre1 = { (float)(cx1 - anchoCaja/2), (float)yNombre, (float)anchoCaja, (float)altoCaja };
    Nombre2 = { (float)(cx2 - anchoCaja/2), (float)yNombre, (float)anchoCaja, (float)altoCaja };

    // Botones Confirmar
    Rectangle confirmar1 = { 50, 480, 200, 40 };
    Rectangle confirmar2 = { 550, 480, 200, 40 };

    // 2. DETECCIÓN DE CLICS
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        
        const int N = 6;
        int numArmas = (int)armasDisponibles.size();

        // LÓGICA JUGADOR 1
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
            // Arma J1
            if (CheckCollisionPointRec(m, j1_arma_izq)) {
                indiceArma1 = (indiceArma1 - 1 + numArmas) % numArmas;
                jugador1.arma = armasDisponibles[indiceArma1];
            }
            if (CheckCollisionPointRec(m, j1_arma_der)) {
                indiceArma1 = (indiceArma1 + 1) % numArmas;
                jugador1.arma = armasDisponibles[indiceArma1];
            }
        }

        // LÓGICA JUGADOR 2
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
            // Arma J2
            if (CheckCollisionPointRec(m, j2_arma_izq)) {
                indiceArma2 = (indiceArma2 - 1 + numArmas) % numArmas;
                jugador2.arma = armasDisponibles[indiceArma2];
            }
            if (CheckCollisionPointRec(m, j2_arma_der)) {
                indiceArma2 = (indiceArma2 + 1) % numArmas;
                jugador2.arma = armasDisponibles[indiceArma2];
            }
        }

        // Nombres
        if (CheckCollisionPointRec(m, Nombre1) && confirmarJugador1 == false) editActivo = 0;
        else if (CheckCollisionPointRec(m, Nombre2) && confirmarJugador2 == false) editActivo = 1;
        else editActivo = -1;

        // Confirmar
        if (CheckCollisionPointRec(m, confirmar1)) {
            confirmarJugador1 = !confirmarJugador1;
            colorConfirmar1 = confirmarJugador1 ? GREEN : RED;
        }
        if (CheckCollisionPointRec(m, confirmar2)) {
            confirmarJugador2 = !confirmarJugador2;
            colorConfirmar2 = confirmarJugador2 ? GREEN : RED;
        }

        // Jugar y Volver
        float anchoJugar = 200; float altoJugar = 60;
        Rectangle jugar = { (float)(GetScreenWidth() - anchoJugar)/2, (float)(GetScreenHeight() - altoJugar)/2, anchoJugar, altoJugar };
        if (CheckCollisionPointRec(m, jugar) && confirmarJugador1 && confirmarJugador2) {
            this->state_machine->add_state(std::make_unique<MainGameState>(jugador1, jugador2), true);
        }

        Rectangle volver = { 20, 20, 120, 40 };
        if (CheckCollisionPointRec(m, volver)) {
            this->state_machine->add_state(std::make_unique<InicioState>(), true);
            return;
        }
    }

    // Teclado
    if (editActivo != -1) {
        int ch;
        while ((ch = GetCharPressed()) > 0) {
            if (ch >= 32 && ch <= 126) {
                if (editActivo == 0 && (int)jugador1.nombre.size() < maxNombre) jugador1.nombre.push_back((char)ch);
                else if (editActivo == 1 && (int)jugador2.nombre.size() < maxNombre) jugador2.nombre.push_back((char)ch);
            }
        }
        if (IsKeyPressed(KEY_BACKSPACE)) {
            if (editActivo == 0 && !jugador1.nombre.empty()) jugador1.nombre.pop_back();
            else if (editActivo == 1 && !jugador2.nombre.empty()) jugador2.nombre.pop_back();
        }
        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_ESCAPE)) editActivo = -1;
    }

    // Transiciones
    if (confirmarJugador1 && confirmarJugador2) {
        mostrandoJugar = true;
        if (transicionAlpha < 1.0f) transicionAlpha += deltaTime * 5.0f;
    } else {
        mostrandoJugar = false;
        if (transicionAlpha > 0.0f) transicionAlpha -= deltaTime * 5.0f;
    }
}

void PersonajesState::render() {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    
    DrawTexturePro(fondo, { 0, 0, (float)fondo.width, (float)fondo.height },
                   { 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() }, { 0, 0 }, 0.0f, WHITE);

    Vector2 mouse = GetMousePosition();
    int cuadrado = 100;
    int y = 200;
    int x1 = 100; int x2 = 600;

    // Personajes
    DrawTexturePro(jugador1.personaje, { 0, 0, (float)jugador1.personaje.width, (float)jugador1.personaje.height },
                   { (float)x1, (float)y, (float)cuadrado, (float)cuadrado }, { 0, 0 }, 0.0f, WHITE);
    DrawTexturePro(jugador2.personaje, { 0, 0, (float)jugador2.personaje.width, (float)jugador2.personaje.height },
                   { (float)x2, (float)y, (float)cuadrado, (float)cuadrado }, { 0, 0 }, 0.0f, WHITE);

    // Nombres
    int anchoCaja = 200; int altoCaja = 40; int sepY = 20;
    int cx1 = x1 + cuadrado/2; int cx2 = x2 + cuadrado/2;
    int yNombre = y - sepY - altoCaja;
    Nombre1 = { (float)(cx1 - anchoCaja/2), (float)yNombre, (float)anchoCaja, (float)altoCaja };
    Nombre2 = { (float)(cx2 - anchoCaja/2), (float)yNombre, (float)anchoCaja, (float)altoCaja };

    DrawRectangleRec(Nombre1, LIGHTGRAY);
    DrawRectangleLinesEx(Nombre1, 2, (editActivo==0)? BLACK : DARKGRAY);
    DrawRectangleRec(Nombre2, LIGHTGRAY);
    DrawRectangleLinesEx(Nombre2, 2, (editActivo==1)? BLACK : DARKGRAY);

    DrawText(jugador1.nombre.c_str(), (int)Nombre1.x + 10, (int)Nombre1.y + 10, 20, BLACK);
    DrawText(jugador2.nombre.c_str(), (int)Nombre2.x + 10, (int)Nombre2.y + 10, 20, BLACK);

    if (editActivo == 0 && fmod(GetTime(), 1.0) < 0.5) 
        DrawRectangle((int)Nombre1.x + 10 + MeasureText(jugador1.nombre.c_str(), 20) + 2, (int)Nombre1.y + 10, 10, 20, BLACK);
    if (editActivo == 1 && fmod(GetTime(), 1.0) < 0.5) 
        DrawRectangle((int)Nombre2.x + 10 + MeasureText(jugador2.nombre.c_str(), 20) + 2, (int)Nombre2.y + 10, 10, 20, BLACK);

    // Flechas SKIN
    int centroY = y + cuadrado / 2;
    Rectangle j1_izq = { (float)(x1 - 45), (float)(centroY - 20), 40, 40 };
    Rectangle j1_der = { (float)(x1 + cuadrado + 5), (float)(centroY - 20), 40, 40 };
    Rectangle j2_izq = { (float)(x2 - 45), (float)(centroY - 20), 40, 40 };
    Rectangle j2_der = { (float)(x2 + cuadrado + 5), (float)(centroY - 20), 40, 40 };

    if(!confirmarJugador1) {
        bool h1 = CheckCollisionPointRec(mouse, j1_izq);
        bool h2 = CheckCollisionPointRec(mouse, j1_der);
        DrawTriangle({ (float)(x1 - 40), (float)centroY }, { (float)(x1 - 10), (float)(centroY + 20) }, { (float)(x1 - 10), (float)(centroY - 20) }, h1 ? DARKGRAY : BLACK);
        DrawTriangle({ (float)(x1 + cuadrado + 40), (float)centroY }, { (float)(x1 + cuadrado + 10), (float)(centroY - 20) }, { (float)(x1 + cuadrado + 10), (float)(centroY + 20) }, h2 ? DARKGRAY : BLACK);
    }
    if(!confirmarJugador2) {
        bool h3 = CheckCollisionPointRec(mouse, j2_izq);
        bool h4 = CheckCollisionPointRec(mouse, j2_der);
        DrawTriangle({ (float)(x2 - 40), (float)centroY }, { (float)(x2 - 10), (float)(centroY + 20) }, { (float)(x2 - 10), (float)(centroY - 20) }, h3 ? DARKGRAY : BLACK);
        DrawTriangle({ (float)(x2 + cuadrado + 40), (float)centroY }, { (float)(x2 + cuadrado + 10), (float)(centroY - 20) }, { (float)(x2 + cuadrado + 10), (float)(centroY + 20) }, h4 ? DARKGRAY : BLACK);
    }

    // --- SELECCIÓN ARMAS ---
float armaAreaWidth = 220.0f;
float armaAreaY = y + cuadrado + 40;

// =======================
// JUGADOR 1
// =======================
Texture2D armaTex1 = jugador1.arma.sprite;

// Escala basada en ancho real
float escala1 = armaAreaWidth / armaTex1.width;
escala1 *= 1.05f;

float anchoFinal1 = armaTex1.width * escala1;
float altoFinal1  = armaTex1.height * escala1;

// Posición centrada + un poco más arriba
float xFinal1 = x1 + (cuadrado - anchoFinal1) / 2;
float yFinal1 = armaAreaY - 20;

// Dibujar arma
DrawTexturePro(
    armaTex1,
    { 0, 0, (float)armaTex1.width, (float)armaTex1.height },
    { xFinal1, yFinal1, anchoFinal1, altoFinal1 },
    { 0, 0 },
    0.0f,
    WHITE
);

// Centro vertical del arma
float armaCentroY1 = yFinal1 + altoFinal1 / 2;

// Flechas más separadas
float flechaOffsetX = 100.0f;

// Hitboxes flechas J1
Rectangle rectIzq1 = { x1 - 120, armaCentroY1 - 20, 40, 40 };
Rectangle rectDer1 = { x1 + cuadrado + 80, armaCentroY1 - 20, 40, 40 };

bool hA1 = CheckCollisionPointRec(mouse, rectIzq1);
bool hA2 = CheckCollisionPointRec(mouse, rectDer1);

// Flechas J1
if (!confirmarJugador1) {
    DrawTriangle(
        { x1 - flechaOffsetX, armaCentroY1 },
        { x1 - flechaOffsetX + 20, armaCentroY1 + 15 },
        { x1 - flechaOffsetX + 20, armaCentroY1 - 15 },
        hA1 ? ROJO_CLARO : RED
    );

    DrawTriangle(
        { x1 + cuadrado + flechaOffsetX, armaCentroY1 },
        { x1 + cuadrado + flechaOffsetX - 20, armaCentroY1 - 15 },
        { x1 + cuadrado + flechaOffsetX - 20, armaCentroY1 + 15 },
        hA2 ? ROJO_CLARO : RED
    );
}

// =======================
// JUGADOR 2
// =======================
Texture2D armaTex2 = jugador2.arma.sprite;

float escala2 = armaAreaWidth / armaTex2.width;
escala2 *= 1.05f;

float anchoFinal2 = armaTex2.width * escala2;
float altoFinal2  = armaTex2.height * escala2;

float xFinal2 = x2 + (cuadrado - anchoFinal2) / 2;
float yFinal2 = armaAreaY - 20;

DrawTexturePro(
    armaTex2,
    { 0, 0, (float)armaTex2.width, (float)armaTex2.height },
    { xFinal2, yFinal2, anchoFinal2, altoFinal2 },
    { 0, 0 },
    0.0f,
    WHITE
);

float armaCentroY2 = yFinal2 + altoFinal2 / 2;

// Hitboxes flechas J2
Rectangle rectIzq2 = { x2 - 120, armaCentroY2 - 20, 40, 40 };
Rectangle rectDer2 = { x2 + cuadrado + 80, armaCentroY2 - 20, 40, 40 };

bool hA3 = CheckCollisionPointRec(mouse, rectIzq2);
bool hA4 = CheckCollisionPointRec(mouse, rectDer2);

// Flechas J2
if (!confirmarJugador2) {
    DrawTriangle(
        { x2 - flechaOffsetX, armaCentroY2 },
        { x2 - flechaOffsetX + 20, armaCentroY2 + 15 },
        { x2 - flechaOffsetX + 20, armaCentroY2 - 15 },
        hA3 ? ROJO_CLARO : RED
    );

    DrawTriangle(
        { x2 + cuadrado + flechaOffsetX, armaCentroY2 },
        { x2 + cuadrado + flechaOffsetX - 20, armaCentroY2 - 15 },
        { x2 + cuadrado + flechaOffsetX - 20, armaCentroY2 + 15 },
        hA4 ? ROJO_CLARO : RED
    );
}

    // Botones Confirmar
    float yConfirmar = 480; 
    float anchoConfirmar = 200; float altoConfirmar = 40;
    
    Rectangle confirmar1 ={50, yConfirmar, anchoConfirmar, altoConfirmar};
    Rectangle confirmar2 ={550, yConfirmar, anchoConfirmar, altoConfirmar};

    DrawRectangleRounded(confirmar1, 0.3f, 8, colorConfirmar1);
    DrawRectangleRounded(confirmar2, 0.3f, 8, colorConfirmar2);

    float fuenteConfirmar = 24.0f;
    Vector2 longConf = MeasureTextEx(poppins, _("Confirmar"), fuenteConfirmar, 2.0f);
    DrawTextEx(poppins, _("Confirmar"), { 50 + (anchoConfirmar - longConf.x) / 2, yConfirmar + (altoConfirmar - longConf.y) / 2 }, fuenteConfirmar, 2.0f, RAYWHITE);
    DrawTextEx(poppins, _("Confirmar"), { 550 + (anchoConfirmar - longConf.x) / 2, yConfirmar + (altoConfirmar - longConf.y) / 2 }, fuenteConfirmar, 2.0f, RAYWHITE);

    // Jugar
    if(transicionAlpha > 0.0f){
        float anchoJugar = 200; float altoJugar = 60;
        Rectangle jugar = { (float)(GetScreenWidth() - anchoJugar) / 2, (float)(GetScreenHeight() - altoJugar) / 2, anchoJugar, altoJugar };
        bool hover = CheckCollisionPointRec(mouse, jugar);
        DrawRectangleRounded(jugar, 0.3f, 8, Fade(hover ? GREEN : GRAY, transicionAlpha));
        
        float fuenteJugar = 35.0f;
        Vector2 longJugar = MeasureTextEx(poppins, _("Jugar"), fuenteJugar, 2.0f);
        DrawTextEx(poppins, _("Jugar"), { jugar.x + (anchoJugar - longJugar.x) / 2, jugar.y + (altoJugar - longJugar.y) / 2 }, fuenteJugar, 2.0f, Fade(RAYWHITE, transicionAlpha));
    }

    // Volver
    Rectangle volver = { 20, 20, 120, 40 };
    bool hoverVolver = CheckCollisionPointRec(mouse, volver);
    DrawRectangleRounded(volver, 0.3f, 8, hoverVolver ? MAROON : RED);
    Vector2 txtVolver = MeasureTextEx(poppins, _("Volver"), 22.0f, 2.0f);
    DrawTextEx(poppins, _("Volver"), { 20 + (120 - txtVolver.x) / 2, 20 + (40 - txtVolver.y) / 2 }, 22.0f, 2.0f, RAYWHITE);

    EndDrawing();
}

void PersonajesState::pause(){

}

void PersonajesState::resume(){
    
}
