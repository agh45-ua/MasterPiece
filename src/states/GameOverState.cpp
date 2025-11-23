extern "C" {
    #include <raylib.h>
}
#include "../core/config.h"
#include "StateMachine.hpp"
#include "MainGameState.hpp"
#include "GameOverState.hpp"
#include "GameState.hpp"
#include <algorithm>
#include <iostream>
#include "InicioState.hpp"

using namespace std;

GameOverState::GameOverState(const Jugador& a, const Jugador& b, const int winnerId, time_t g_time)
: jugador1(a), jugador2(b), winner_id(winnerId), game_time(g_time) {}

void GameOverState::init(){
    fondo = LoadTexture(GetAssetPath("fondo-juego.png").c_str());
}

void GameOverState::handleInput(){
    if(IsKeyPressed(KEY_SPACE)){
        this->state_machine->add_state(std::make_unique<MainGameState>(jugador1, jugador2), true);
    } else if (IsKeyPressed(KEY_ESCAPE)){
        this->state_machine->add_state(std::make_unique<InicioState>(), true);
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

    const char* title = "GAME OVER";
    const int titleSize = 52;
    const int titleWidth = MeasureText(title, titleSize);

    string winnerText = "Ganador: " + (winner_id == 1 ? jugador1.nombre : jugador2.nombre);
    const int winnerSize = 42;
    const int winnerWidth = MeasureText(winnerText.c_str(), winnerSize);

    const char* subText = "¡Enhorabuena!";
    const int subSize = 28;
    const int subWidth = MeasureText(subText, subSize);

    string gameTimeText = string("Tiempo de juego: ") + to_string(game_time/60) + "m " + to_string(game_time%60) + "s";
    const int gameTimeSize = 22;
    const int gameTimeWidth = MeasureText(gameTimeText.c_str(), gameTimeSize);

    const char* restartHint = "Pulsa ESPACIO para jugar de nuevo";
    const char* exitHint = "Pulsa ESCAPE para volver al inicio";
    const int hintSize = 22;
    const int hintWidth = MeasureText(restartHint, hintSize);

    int maxContentWidth = titleWidth;
    maxContentWidth = std::max(maxContentWidth, winnerWidth);
    maxContentWidth = std::max(maxContentWidth, subWidth);
    maxContentWidth = std::max(maxContentWidth, hintWidth);

    const float horizontalPadding = 120.0f;
    const float panelMinWidth = 440.0f;
    const float panelWidth = std::max(panelMinWidth, static_cast<float>(maxContentWidth) + horizontalPadding);

    const float topPadding = 40.0f;
    const float spacingTitleWinner = 38.0f;
    const float spacingWinnerSub = 26.0f;
    const float spacingSubHint = 40.0f;
    const float bottomPadding = 120.0f;
    const float panelMinHeight = 300.0f;
    const float requiredHeight = topPadding + titleSize + spacingTitleWinner + winnerSize +
                                 spacingWinnerSub + subSize + spacingSubHint + hintSize + bottomPadding;
    const float panelHeight = std::max(panelMinHeight, requiredHeight);

    Rectangle panel = {centerX - panelWidth / 2.0f, centerY - panelHeight / 2.0f, panelWidth, panelHeight};
    DrawRectangleRounded(panel, 0.18f, 12, panelColor);
    DrawRectangleRoundedLines(panel, 0.18f, 12, winnerColor);

    const float panelCenterX = panel.x + panel.width / 2.0f;

    float currentY = panel.y + topPadding;
    DrawText(title, static_cast<int>(panelCenterX - titleWidth / 2), static_cast<int>(currentY), titleSize, RAYWHITE);

    currentY += titleSize + spacingTitleWinner;
    DrawText(winnerText.c_str(), static_cast<int>(panelCenterX - winnerWidth / 2), static_cast<int>(currentY), winnerSize, winnerColor);

    currentY += winnerSize + spacingWinnerSub;
    DrawText(subText, static_cast<int>(panelCenterX - subWidth / 2), static_cast<int>(currentY), subSize, RAYWHITE);

    currentY += subSize + spacingSubHint;
    DrawText(gameTimeText.c_str(), static_cast<int>(panelCenterX - gameTimeWidth / 2), static_cast<int>(currentY), gameTimeSize, LIGHTGRAY);

    currentY += gameTimeSize + 12;
    DrawText(restartHint, static_cast<int>(panelCenterX - hintWidth / 2), static_cast<int>(currentY), hintSize, LIGHTGRAY);

    currentY += hintSize + 12;
    DrawText(exitHint, static_cast<int>(panelCenterX - hintWidth / 2), static_cast<int>(currentY), hintSize, LIGHTGRAY);

    EndDrawing();
}

void GameOverState::pause(){

}

void GameOverState::resume(){
    
}
