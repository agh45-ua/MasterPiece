#include "MainGameState.hpp"
#include "StateMachine.hpp"
#include "GameOverState.hpp"
#include <string>
#include <iostream>
extern "C" {
    #include <raylib.h>
}

MainGameState::MainGameState(){
}

void MainGameState::init(){
    ground = {0, (float)screenHeight - groundHeight, (float)screenWidth, (float)groundHeight};
    player1 = {100, ground.y - 50, 50, 50};
    player2 = {screenWidth - 150.0f, ground.y - 50, 50, 50};
    

    projectile_1 = {{0,0},{0,0}, false};
    angle_1= -30.0f * DEG2RAD;
    angle_2= -30.0f * DEG2RAD;
    
    //para  la primera iteracion
    old_player1=player1;
    old_player2=player2;

    turno ='1';
}

void MainGameState::handleInput(){
  
    //si es el turno del jugador 1 vamos guardando lo que hace hasta que presione enter
    if (turno =='1'){
        
        old_angle_1=angle_1;
        // Calcular ángulo entre el jugador1 y el ratón
        Vector2 mousePos = GetMousePosition();
        Vector2 start = {player1.x + player1.width, player1.y + player1.height / 2};
        angle_1= atan2f(mousePos.y - start.y, mousePos.x - start.x);


    
        // Disparo con ESPACIO o clic izquierdo
        if ((IsKeyPressed(KEY_SPACE) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON))) {

            
            projectile_1.pos = start;
            float speed = 500.0f;
            projectile_1.vel = {speed * cosf(angle_1), speed * sinf(angle_1)};
        }

        // Movimiento del jugador
        if (IsKeyDown(KEY_A) && player1.x > 0) player1.x -= 200 * GetFrameTime();
        if (IsKeyDown(KEY_D) && player1.x + player1.width < screenWidth) player1.x += 200 * GetFrameTime();
        
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
        Vector2 start = {player2.x + player2.width, player2.y + player2.height / 2};
        angle_2= atan2f(mousePos.y - start.y, mousePos.x - start.x);


    
        // Disparo con ESPACIO o clic izquierdo
        if ((IsKeyPressed(KEY_SPACE) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON))) {

            projectile_2.pos = start;
            float speed = 500.0f;
            projectile_2.vel = {speed * cosf(angle_2), speed * sinf(angle_2)};
            projectile_2.active = false;
        }

         // Movimiento del jugador
        if (IsKeyDown(KEY_A) && player2.x > 0) player2.x -= 200 * GetFrameTime();
        if (IsKeyDown(KEY_D) && player2.x + player2.width < screenWidth) player2.x += 200 * GetFrameTime();

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
        projectile_1.active = true;
        projectile_2.active = true;

        projectile_1.vel.y += gravity_1 * deltaTime;
        projectile_1.pos.x += projectile_1.vel.x * deltaTime;
        projectile_1.pos.y += projectile_1.vel.y * deltaTime;

        projectile_2.vel.y += gravity_2 * deltaTime;
        projectile_2.pos.x += projectile_2.vel.x * deltaTime;
        projectile_2.pos.y += projectile_2.vel.y * deltaTime;
        // Colisión con el suelo
        if (projectile_1.pos.y > ground.y) {
            projectile_1.active = false;
        }
         if (projectile_2.pos.y > ground.y) {
            projectile_2.active = false;
        }
        // Colisión con jugadores

        if (CheckCollisionPointRec(projectile_1.pos, player2) || CheckCollisionPointRec(projectile_2.pos, player1)) {
            projectile_1.active = false;
            projectile_2.active = false;

            // Cambiar al estado GameOver
            this->state_machine->add_state(make_unique<GameOverState>(), true);
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

    if (countdownActive) {
        int secondsLeft = (int)ceil(countdownTime);
        string s = to_string(secondsLeft);
        DrawText(s.c_str(), screenWidth/2 - 20, screenHeight/2 - 20, 50, PURPLE);
    } else {
        //Turno
        string s1 = "Turno "+ to_string( contador_turno),s2 = "Turno de "+ string(1, turno);
        DrawText(s1.c_str(),screenWidth/2,screenHeight/2,24,PURPLE);
        DrawText(s2.c_str(),100,screenHeight/2,24,PURPLE);
        // Suelo
        DrawRectangleRec(ground, DARKGREEN);

        // Jugadores para que sean ocultos los movimientos
        switch (turno)
        {
            case '1':
            DrawRectangleRec(player1, BLUE);
            DrawRectangleRec(old_player2, RED);
            break;

            case '2':
            DrawRectangleRec(old_player1, BLUE);
            DrawRectangleRec(player2, RED);
            break;

            case 'r':
            DrawRectangleRec(player1, BLUE);
            DrawRectangleRec(player2, RED);
            break;
        }
        

        // Flecha de apuntado
        if(turno=='1' || turno=='r'){//flecha j1
            Vector2 start = {player1.x + player1.width, player1.y + player1.height / 2};
            Vector2 end = {start.x + arrowLength_1 * cosf(angle_1), start.y + arrowLength_1 * sinf(angle_1)};
            DrawLineEx(start, end, 4, DARKGRAY);
            DrawTriangle(
                end,
                {end.x - 10 * cosf(angle_1- 0.3f), end.y - 10 * sinf(angle_1- 0.3f)},
                {end.x - 10 * cosf(angle_1+ 0.3f), end.y - 10 * sinf(angle_1+ 0.3f)},
                DARKGRAY
            );
        }
        if(turno=='2' || turno=='r'){//flecha j2
            Vector2 start2 = {player2.x + player2.width, player2.y + player2.height / 2};
            Vector2 end2 = {start2.x + arrowLength_2 * cosf(angle_2), start2.y + arrowLength_2 * sinf(angle_2)};
            DrawLineEx(start2, end2, 4, DARKGRAY);
            DrawTriangle(
                end2,
                {end2.x - 10 * cosf(angle_2- 0.3f), end2.y - 10 * sinf(angle_2- 0.3f)},
                {end2.x - 10 * cosf(angle_2+ 0.3f), end2.y - 10 * sinf(angle_2+ 0.3f)},
                DARKGRAY
            );
        }

        // Proyectiles cuando sea resolucion
        if (turno=='r'){
            if(projectile_1.active) DrawCircleV(projectile_1.pos, 5, BLACK);

            if (projectile_2.active) DrawCircleV(projectile_2.pos, 5, BLACK);

        }

        DrawText("W/S para apuntar", 20, 20, 20, DARKGRAY);
        DrawText("ESPACIO para disparar", 20, 45, 20, DARKGRAY);
    }
    EndDrawing();
}