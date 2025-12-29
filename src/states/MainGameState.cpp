#include "MainGameState.hpp"
#include "StateMachine.hpp"
#include "GameOverState.hpp"
#include <string>
#include <iostream>
#include "../core/config.h"
extern "C" {
    #include <raylib.h>
}

using namespace std;

MainGameState::MainGameState(const Jugador& a, const Jugador& b)
: jugador1(a), jugador2(b) {}

void MainGameState::init(){
    fondo = LoadTexture(GetAssetPath("fondo-juego.png").c_str());
    ground = {0, (float)screenHeight - groundHeight, (float)screenWidth, (float)groundHeight};
    player1 = {{100, ground.y - 50, 50, 50}, 100};
    player2 = {{screenWidth - 150.0f, ground.y - 50, 50, 50}, 100};

    projectile_1 = {{0,0},{0,0}, false, false};
    projectile_2 = {{0,0},{0,0}, false, false};
    angle_1= -30.0f * DEG2RAD;
    angle_2= -30.0f * DEG2RAD;
    
    //para  la primera iteracion
    old_player1=player1;
    old_player2=player2;

    turno ='1';
    startTime = time(nullptr);
}

void MainGameState::handleInput(){
  
    //si es el turno del jugador 1 vamos guardando lo que hace hasta que presione enter
    if (turno =='1'){
        old_angle_1=angle_1;
        // Calcular ángulo entre el jugador1 y el ratón
        Vector2 mousePos = GetMousePosition();
        Vector2 start = {player1.rect.x + player1.rect.width, player1.rect.y + player1.rect.height / 2};
        angle_1= atan2f(mousePos.y - start.y, mousePos.x - start.x);

        // Disparo con ESPACIO o clic izquierdo
        if ((IsKeyPressed(KEY_SPACE) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON))) {
            projectile_1.pos = start;
            
            // Velocidad del arma elegida
            float speed = jugador1.arma.speed;
        
            projectile_1.vel = {speed * cosf(angle_1), speed * sinf(angle_1)};
            projectile_1.active = true;
            projectile_1.hasHit = false;
        }

        // Movimiento del jugador
        if (IsKeyDown(KEY_A) && player1.rect.x > 0) player1.rect.x -= 200 * GetFrameTime();
        if (IsKeyDown(KEY_D) && player1.rect.x + player1.rect.width < screenWidth) player1.rect.x += 200 * GetFrameTime();
        
        //ya no pillamos mas info del jugador 1 al pulsar enter
        if(IsKeyPressed(KEY_ENTER)){
            turno='2';
           
        }
    }
    /////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////
    //si es el turno del jugador 2 vamos guardando lo que hace hasta que presione enter
    if (turno =='2'){
        old_angle_2=angle_2;
        // Calcular ángulo entre el jugador2 y el ratón
        Vector2 mousePos = GetMousePosition();
        Vector2 start = {player2.rect.x + player2.rect.width, player2.rect.y + player2.rect.height / 2};
        angle_2= atan2f(mousePos.y - start.y, mousePos.x - start.x);


    
        // Disparo con ESPACIO o clic izquierdo
        if ((IsKeyPressed(KEY_SPACE) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON))) {
            projectile_2.pos = start;

            float speed = jugador2.arma.speed;

            projectile_2.vel = {speed * cosf(angle_2), speed * sinf(angle_2)};
            projectile_2.active = true;
            projectile_2.hasHit = false;
        }

         // Movimiento del jugador
        if (IsKeyDown(KEY_A) && player2.rect.x > 0) player2.rect.x -= 200 * GetFrameTime();
        if (IsKeyDown(KEY_D) && player2.rect.x + player2.rect.width < screenWidth) player2.rect.x += 200 * GetFrameTime();

        //ya no pillamos mas info del jugador 2
        if(IsKeyPressed(KEY_Q)){
            countdownActive = true;
            countdownTime = 3.0f;  // reiniciar cuenta atrás
            gameBlocked = true;    // bloquea el resto del juego
            turno='r';
        }
    }
}

void MainGameState::update(float deltaTime){

    if (countdownActive) {
        countdownTime -= deltaTime;
        if (countdownTime <= 0) {
            countdownTime = 0;
            countdownActive = false;
            gameBlocked = false;  // desbloquea el juego
        }
        return; // mientras la cuenta atrás está activa, no actualizamos nada más
    }

    // --- LOGICA ---
    if (turno=='r') { //Actualizamos los proyectiles cuando se entre en la resolucion del turno

        projectile_1.vel.y += jugador1.arma.gravity * deltaTime;
        projectile_1.pos.x += projectile_1.vel.x * deltaTime;
        projectile_1.pos.y += projectile_1.vel.y * deltaTime;

        projectile_2.vel.y += jugador2.arma.gravity * deltaTime;
        projectile_2.pos.x += projectile_2.vel.x * deltaTime;
        projectile_2.pos.y += projectile_2.vel.y * deltaTime;
        
        //Comprobar limite pantalla
        if (projectile_1.pos.x < 0 || projectile_1.pos.x > screenWidth || projectile_1.pos.y > screenHeight){
            projectile_1.active = false;
        }
        if (projectile_2.pos.x < 0 || projectile_2.pos.x > screenWidth || projectile_2.pos.y > screenHeight) {
            projectile_2.active = false;
        }

        // Colisión con el suelo
        if (projectile_1.pos.y > ground.y) {
            projectile_1.active = false;
        }
        if (projectile_2.pos.y > ground.y) {
            projectile_2.active = false;
        }

        // Colisión con jugadores
        if (!projectile_1.hasHit && CheckCollisionPointRec(projectile_1.pos, player2.rect)) {
            projectile_1.active = false;
            projectile_1.hasHit = true;
            player2.health -= jugador1.arma.damage;
        }

        if (!projectile_2.hasHit && CheckCollisionPointRec(projectile_2.pos, player1.rect)) {
            projectile_2.active = false;
            projectile_2.hasHit = true;
            player1.health -= jugador2.arma.damage;
        }

        int winnerId = 0;
        //Si alguno de los dos jugadores pierde toda la vida cambiamos estado
        if (player1.health <= 0 || player2.health <= 0) {
            if(player1.health <= 0){
                winnerId = 2;
            }
            else{
                if(player2.health <= 0){
                    winnerId = 1;
                }
            }

            this->state_machine->add_state(
                std::make_unique<GameOverState>(
                    jugador1,
                    jugador2,
                    winnerId,
                    static_cast<time_t>(difftime(time(nullptr), startTime))
                ),
                true
            );
        }

        //cuando los projectile acaben(false) siguiente turno y le toca al j1
        if (projectile_1.active == false && projectile_2.active == false){ 
            old_player1=player1;
            old_player2=player2;
            contador_turno++;
            turno='1';
        }
    }
}

void MainGameState::render(){
    
    BeginDrawing();
    
    ClearBackground(RAYWHITE);
    DrawTexturePro(
        fondo,
        { 0, 0, (float)fondo.width, (float)fondo.height },
        { 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() },
        { 0, 0 },
        0.0f,
        Fade(WHITE, 0.8f)
    );
    if (countdownActive) {
        int secondsLeft = (int)ceil(countdownTime);
        string s = to_string(secondsLeft);
        DrawText(s.c_str(), screenWidth/2 - 20, screenHeight/2 - 20, 50, PURPLE);
    } else {
        //Turno
        string s1 = "Turno "+ to_string( contador_turno),s2 = "Turno de "+ string(1, turno);
        DrawText(s1.c_str(),screenWidth/2,screenHeight/2,24,PURPLE);
        DrawText(s2.c_str(),100,screenHeight/2,24,PURPLE);
        // Jugadores para que sean ocultos los movimientos
        auto src1 = Rectangle{0, 0, (float)jugador1.personaje.width, (float)jugador1.personaje.height};
        auto src2 = Rectangle{0, 0, (float)jugador2.personaje.width, (float)jugador2.personaje.height};

        switch (turno)
        {
            case '1':
                // J1 activo, J2 “fantasma”
                DrawTexturePro(jugador1.personaje, src1, player1.rect, {0,0}, 0.0f, WHITE);
                DrawTexturePro(jugador2.personaje, src2, old_player2.rect, {0,0}, 0.0f, Fade(WHITE, 0.5f));
                break;

            case '2':
                // J2 activo, J1 “fantasma”
                DrawTexturePro(jugador1.personaje, src1, old_player1.rect, {0,0}, 0.0f, Fade(WHITE, 0.5f));
                DrawTexturePro(jugador2.personaje, src2, player2.rect, {0,0}, 0.0f, WHITE);
                break;

            case 'r':
                // Ambos activos
                DrawTexturePro(jugador1.personaje, src1, player1.rect, {0,0}, 0.0f, WHITE);
                DrawTexturePro(jugador2.personaje, src2, player2.rect, {0,0}, 0.0f, WHITE);
                break;
        }

        if (turno == '1' || turno == 'r') {

            Texture2D arma = jugador1.arma.sprite;

            Vector2 mano = {
                player1.rect.x + player1.rect.width,
                player1.rect.y + player1.rect.height * 0.55f
            };

            float escala = (player1.rect.height * 0.9f) / arma.height;
            float ancho  = arma.width  * escala;
            float alto   = arma.height * escala;

            bool apuntaAtras = cosf(angle_1) < 0;

            Rectangle src;
            if (!apuntaAtras) {
                src = { 0, 0, (float)arma.width, (float)arma.height };
            } else {
                src = { 0, (float)arma.height, (float)arma.width, -(float)arma.height };
            }

            Rectangle dst = { mano.x, mano.y, ancho, alto };
            Vector2 origin = { 0.0f, alto * 0.5f };

            DrawTexturePro(
                arma,
                src,
                dst,
                origin,
                angle_1 * RAD2DEG,
                WHITE
            );
        }

        if (turno == '2' || turno == 'r') {

            Texture2D arma = jugador2.arma.sprite;

            Vector2 mano = {
                player2.rect.x,
                player2.rect.y + player2.rect.height * 0.55f
            };

            float escala = (player2.rect.height * 0.9f) / arma.height;
            float ancho  = arma.width  * escala;
            float alto   = arma.height * escala;

            bool apuntaAtras = cosf(angle_2) < 0;

            Rectangle src;
            if (!apuntaAtras) {
                src = { 0, 0, (float)arma.width, (float)arma.height };
            } else {
                src = { 0, (float)arma.height, (float)arma.width, -(float)arma.height };
            }

            Rectangle dst = { mano.x, mano.y, ancho, alto };
            Vector2 origin = { 0.0f, alto * 0.5f };

            DrawTexturePro(
                arma,
                src,
                dst,
                origin,
                angle_2 * RAD2DEG,
                WHITE
            );
        }

        // Proyectiles cuando sea resolucion
        if (turno=='r'){
            if(projectile_1.active) DrawCircleV(projectile_1.pos, 5, RED);
            if (projectile_2.active) DrawCircleV(projectile_2.pos, 5, RED);

        }
        
        // --- Barras de vida (HUD) ---
        float maxBarWidth = 200;
        float barHeight = 20;

        // Jugador 1 - esquina superior izquierda
        DrawText("Jugador 1", 30, 20, 20, BLACK);
        DrawRectangle(30, 50, maxBarWidth, barHeight, GRAY);
        DrawRectangle(30, 50, maxBarWidth * (player1.health / 100.0f), barHeight, GREEN);
        DrawText(TextFormat("%d / 100", player1.health), 30, 50 + barHeight + 5, 20, DARKGREEN);

        // Jugador 2 - esquina superior derecha
        DrawText("Jugador 2", screenWidth - 230, 20, 20, BLACK);
        DrawRectangle(screenWidth - 230, 50, maxBarWidth, barHeight, GRAY);
        DrawRectangle(screenWidth - 230 + (maxBarWidth * (1 - player2.health / 100.0f)), 50,
                    maxBarWidth * (player2.health / 100.0f), barHeight, GREEN);
        DrawText(TextFormat("%d / 100", player2.health), screenWidth - 230, 50 + barHeight + 5, 20, DARKGREEN);

        //DrawText("ESPACIO para disparar", 20, 45, 20, DARKGRAY);
    }
    EndDrawing();
}
