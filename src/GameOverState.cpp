extern "C" {
    #include <raylib.h>
}
#include "StateMachine.hpp"
#include "MainGameState.hpp"
#include "GameOverState.hpp"
#include "GameState.hpp"
#include <iostream>

GameOverState::GameOverState(int winnerId): winner_id(winnerId){}
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

    ClearBackground(BLACK);
    DrawTexturePro(
        fondo,
        { 0, 0, (float)fondo.width, (float)fondo.height },
        { 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() },
        { 0, 0 },
        0.0f,
        WHITE
    );

    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();
    const int centerX = screenWidth / 2;
    const int centerY = screenHeight / 2;

    Color winnerColor = (winner_id == 1) ? SKYBLUE : RED;
    Color panelColor = Fade(DARKGRAY, 0.85f);

    DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.6f));
    DrawCircleGradient(centerX, centerY, 260.0f, Fade(winnerColor, 0.45f), Fade(BLACK, 0.0f));

    Rectangle panel = {centerX - 220.0f, centerY - 150.0f, 440.0f, 300.0f};
    DrawRectangleRounded(panel, 0.18f, 12, panelColor);
    DrawRectangleRoundedLines(panel, 0.18f, 12, winnerColor);

    const char* title = "GAME OVER";
    const int titleSize = 52;
    const int titleWidth = MeasureText(title, titleSize);
    DrawText(title, centerX - titleWidth / 2, centerY - 120, titleSize, RAYWHITE);

    string winnerText = "Ganador: Jugador " + to_string(winner_id);
    const int winnerSize = 42;
    const int winnerWidth = MeasureText(winnerText.c_str(), winnerSize);
    DrawText(winnerText.c_str(), centerX - winnerWidth / 2, centerY - 30, winnerSize, winnerColor);

    const char* subText = "¡Enhorabuena!";
    const int subSize = 28;
    const int subWidth = MeasureText(subText, subSize);
    DrawText(subText, centerX - subWidth / 2, centerY + 20, subSize, RAYWHITE);

    const char* restartHint = "Pulsa ESPACIO para jugar de nuevo";
    const int hintSize = 22;
    const int hintWidth = MeasureText(restartHint, hintSize);
    DrawText(restartHint, centerX - hintWidth / 2, centerY + 80, hintSize, LIGHTGRAY);

    EndDrawing();
}

void GameOverState::pause(){

}

void GameOverState::resume(){
    
}
