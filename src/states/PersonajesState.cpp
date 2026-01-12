#include "InicioState.hpp"
#include "MainGameState.hpp"
#include "PersonajesState.hpp"
#include "StateMachine.hpp"
#include "GameState.hpp"
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
    fondo = LoadTexture(GetAssetPath("fondo-juego.png").c_str());
    poppins = LoadFontEx(GetAssetPath("Poppins-Bold.ttf").c_str(), 120, 0, 0);
    
    // INICIALIZAMOS VARIABLES
    color1 = 0;
    color2 = 1;
    confirmarJugador1 = false;
    confirmarJugador2 = false;
    colorConfirmar1 = GRAY;
    colorConfirmar2 = GRAY;

    // TEXTURAS PERSONAJES
    for (int i = 0; i < 6; i++) {
        personajes[i] = LoadTexture(GetAssetPath(rutas[i]).c_str());
    }

    // DEFINICIÓN DE ARMAS
    armasDisponibles = {
        Arma{_("Estandar"), 34, 500.0f, 400.0f, BLACK},
        Arma{_("Sniper"), 25, 1000.0f, 50.0f, RED},       // Rápida, poca caída, menos daño
        Arma{_("Mortero"), 50, 400.0f, 900.0f, DARKGREEN}, // Mucho daño, mucha gravedad
        Arma{_("Pesada"), 40, 300.0f, 200.0f, PURPLE}      // Lenta, flota más, buen daño
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

    // 1. DEFINICIÓN DE POSICIONES
    int cuadrado = 100;
    int y = 200;
    int x1 = 100;
    int x2 = 600;
    int centroY = y + cuadrado / 2;

    // Hitboxes Flechas Personaje (Skin)
    Rectangle j1_izq = { (float)(x1 - 45), (float)(centroY - 20), 40, 40 };
    Rectangle j1_der = { (float)(x1 + cuadrado + 5), (float)(centroY - 20), 40, 40 };
    Rectangle j2_izq = { (float)(x2 - 45), (float)(centroY - 20), 40, 40 };
    Rectangle j2_der = { (float)(x2 + cuadrado + 5), (float)(centroY - 20), 40, 40 }; 

    // --- HITBOXES ARMA (MÁS SEPARADOS) ---
    int yArma = y + 170; 
    
    // Jugador 1
    // Separamos 20px más hacia fuera
    Rectangle j1_arma_izq = { (float)(x1 - 60), (float)(yArma - 5), 40, 40 };
    Rectangle j1_arma_der = { (float)(x1 + 120), (float)(yArma - 5), 40, 40 };
    
    // Jugador 2
    Rectangle j2_arma_izq = { (float)(x2 - 60), (float)(yArma - 5), 40, 40 };
    Rectangle j2_arma_der = { (float)(x2 + 120), (float)(yArma - 5), 40, 40 };

    // Rectángulos Nombres
    int anchoCaja = 200; int altoCaja = 40; int sepY = 20;
    int cx1 = x1 + cuadrado/2; int cx2 = x2 + cuadrado/2;
    int yNombre = y - sepY - altoCaja;
    
    Nombre1 = { (float)(cx1 - anchoCaja/2), (float)yNombre, (float)anchoCaja, (float)altoCaja };
    Nombre2 = { (float)(cx2 - anchoCaja/2), (float)yNombre, (float)anchoCaja, (float)altoCaja };

    // Botones Confirmar
    Rectangle confirmar1 ={50, 420, 200, 40};
    Rectangle confirmar2 ={550, 420, 200, 40};

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
            colorConfirmar1 = confirmarJugador1 ? GREEN : GRAY;
        }
        if (CheckCollisionPointRec(m, confirmar2)) {
            confirmarJugador2 = !confirmarJugador2;
            colorConfirmar2 = confirmarJugador2 ? GREEN : GRAY;
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
    int yArma = y + 170;
    int tamTexto = 30;

    // Cajas arma
    int anchoCajaArma = 200;
    int altoCajaArma = 40;
    int yCaja = y + 120;

    Rectangle cajaTitulo1 = { (float)cx1 - anchoCajaArma/2, (float)yCaja, (float)anchoCajaArma, (float)altoCajaArma };
    Rectangle cajaTitulo2 = { (float)cx2 - anchoCajaArma/2, (float)yCaja, (float)anchoCajaArma, (float)altoCajaArma };

    // -- Jugador 1 --
    DrawRectangleRec(cajaTitulo1, LIGHTGRAY);
    DrawRectangleLinesEx(cajaTitulo1, 2, DARKGRAY);
    DrawText(_("Arma:"), (int)cajaTitulo1.x + (anchoCajaArma - MeasureText(_("Arma:"), 20))/2, (int)cajaTitulo1.y + 10, 20, BLACK);
    const char* nArma1 = jugador1.arma.nombre.c_str();
    int wArma1 = MeasureText(nArma1, tamTexto);
    DrawText(nArma1, x1 + (cuadrado - wArma1)/2, yArma + 5, tamTexto, BLACK);

    if (!confirmarJugador1) {
        // Hitbox separado (coincide con update)
        Rectangle rectIzq = { (float)(x1 - 60), (float)(yArma - 5), 40, 40 };
        Rectangle rectDer = { (float)(x1 + 120), (float)(yArma - 5), 40, 40 };
        bool hA1 = CheckCollisionPointRec(mouse, rectIzq);
        bool hA2 = CheckCollisionPointRec(mouse, rectDer);
        
        // Visual más separado
        // Izq: Punta x-45, Base x-30
        DrawTriangle({(float)x1 - 45, (float)yArma + 15}, {(float)x1 - 30, (float)yArma + 25}, {(float)x1 - 30, (float)yArma + 5}, hA1 ? DARKGRAY : BLACK);
        // Der: Punta x+145, Base x+130
        DrawTriangle({(float)x1 + 145, (float)yArma + 15}, {(float)x1 + 130, (float)yArma + 5}, {(float)x1 + 130, (float)yArma + 25}, hA2 ? DARKGRAY : BLACK);
    }

    // -- Jugador 2 --
    DrawRectangleRec(cajaTitulo2, LIGHTGRAY);
    DrawRectangleLinesEx(cajaTitulo2, 2, DARKGRAY);
    DrawText(_("Arma:"), (int)cajaTitulo2.x + (anchoCajaArma - MeasureText(_("Arma:"), 20))/2, (int)cajaTitulo2.y + 10, 20, BLACK);
    const char* nArma2 = jugador2.arma.nombre.c_str();
    int wArma2 = MeasureText(nArma2, tamTexto);
    DrawText(nArma2, x2 + (cuadrado - wArma2)/2, yArma + 5, tamTexto, BLACK);

    if (!confirmarJugador2) {
        Rectangle rectIzq2 = { (float)(x2 - 60), (float)(yArma - 5), 40, 40 };
        Rectangle rectDer2 = { (float)(x2 + 120), (float)(yArma - 5), 40, 40 };
        bool hA3 = CheckCollisionPointRec(mouse, rectIzq2);
        bool hA4 = CheckCollisionPointRec(mouse, rectDer2);
        
        DrawTriangle({(float)x2 - 45, (float)yArma + 15}, {(float)x2 - 30, (float)yArma + 25}, {(float)x2 - 30, (float)yArma + 5}, hA3 ? DARKGRAY : BLACK);
        DrawTriangle({(float)x2 + 145, (float)yArma + 15}, {(float)x2 + 130, (float)yArma + 5}, {(float)x2 + 130, (float)yArma + 25}, hA4 ? DARKGRAY : BLACK);
    }

    // Botones Confirmar
    float yConfirmar = 420; 
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
