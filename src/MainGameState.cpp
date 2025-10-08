#include "MainGameState.hpp"
#include "StateMachine.hpp"
#include "GameOverState.hpp"
#include <iostream>
extern "C" {
    #include <raylib.h>
}

MainGameState::MainGameState(){
}

void MainGameState::init(){
    ground = {0, (float)screenHeight - groundHeight, (float)screenWidth, (float)groundHeight};
    player1 = {100, ground.y - 50, 50, 50};
    trianglePos = {screenWidth - 150.0f, ground.y - 50};
    triangleSize = 50.0f;

    projectile = {{0,0},{0,0}, false};
    angle = -30.0f * DEG2RAD;
}

void MainGameState::handleInput(){
    /*
    // Apuntado con W/S
    if (IsKeyDown(KEY_W)) angle -= angleSpeed * GetFrameTime();
    if (IsKeyDown(KEY_S)) angle += angleSpeed * GetFrameTime();

    // Limitar el ángulo entre -80° y 0° (para no apuntar hacia abajo)
    if (angle < -80 * DEG2RAD) angle = -80 * DEG2RAD;
    if (angle > 0) angle = 0;

    // Disparo
    if (IsKeyPressed(KEY_SPACE) && !projectile.active) {
        projectile.active = true; //Activamos el proyectil
        projectile.pos = {player1.x + player1.width, player1.y + player1.height / 2}; //Posicionamos el proyectil en el borde derecho del jugador
        float speed = 500.0f; //Velocidad inicial
        projectile.vel = {speed * cosf(angle), speed * sinf(angle)};
    }
    */

    // Calcular ángulo entre el jugador y el ratón
    Vector2 mousePos = GetMousePosition();
    Vector2 start = {player1.x + player1.width, player1.y + player1.height / 2};
    angle = atan2f(mousePos.y - start.y, mousePos.x - start.x);

    // Disparo con ESPACIO o clic izquierdo
    if ((IsKeyPressed(KEY_SPACE) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) && !projectile.active) {

        projectile.active = true;
        projectile.pos = start;
        float speed = 500.0f;
        projectile.vel = {speed * cosf(angle), speed * sinf(angle)};
    }
}

void MainGameState::update(float deltaTime){
    // --- LOGICA ---
    if (projectile.active) { //Actualizamos el proyectil
        projectile.vel.y += gravity * deltaTime;
        projectile.pos.x += projectile.vel.x * deltaTime;
        projectile.pos.y += projectile.vel.y * deltaTime;

        // Colisión con el suelo
        if (projectile.pos.y > ground.y) {
            projectile.active = false;
        }

        // Colisión con triángulo
        Rectangle triBounds = {
            trianglePos.x - triangleSize,
            trianglePos.y - triangleSize,
            triangleSize * 2,
            triangleSize * 2
        };

        if (CheckCollisionPointRec(projectile.pos, triBounds)) {
            projectile.active = false;
          
            // Cambiar al estado GameOver
            this->state_machine->add_state(make_unique<GameOverState>(), true);
        }
    }
}

void MainGameState::render(){
    
    BeginDrawing();
    
    ClearBackground(RAYWHITE);

    // Suelo
    DrawRectangleRec(ground, DARKGREEN);

    // Jugador
    DrawRectangleRec(player1, BLUE);

    // Triángulo enemigo
    DrawTriangle(
        {trianglePos.x, trianglePos.y - triangleSize},
        {trianglePos.x - triangleSize, trianglePos.y + triangleSize},
        {trianglePos.x + triangleSize, trianglePos.y + triangleSize},
        RED
    );

    // Flecha de apuntado
    Vector2 start = {player1.x + player1.width, player1.y + player1.height / 2};
    Vector2 end = {start.x + arrowLength * cosf(angle), start.y + arrowLength * sinf(angle)};
    DrawLineEx(start, end, 4, DARKGRAY);
    DrawTriangle(
        end,
        {end.x - 10 * cosf(angle - 0.3f), end.y - 10 * sinf(angle - 0.3f)},
        {end.x - 10 * cosf(angle + 0.3f), end.y - 10 * sinf(angle + 0.3f)},
        DARKGRAY
    );

    // Proyectil
    if (projectile.active)
        DrawCircleV(projectile.pos, 5, BLACK);

    DrawText("W/S para apuntar", 20, 20, 20, DARKGRAY);
    DrawText("ESPACIO para disparar", 20, 45, 20, DARKGRAY);

    EndDrawing();
}