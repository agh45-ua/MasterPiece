extern "C" {
    #include <raylib.h>
}
#include "../core/config.h"
#include "StateMachine.hpp"
#include "MainGameState.hpp"
#include "GameOverState.hpp"
#include "GameState.hpp"
#include "ResourceManager.hpp"
#include <algorithm>
#include <iostream>
#include "InicioState.hpp"

using namespace std;

GameOverState::GameOverState(const Jugador& a, const Jugador& b, const int winnerId, time_t g_time)
: jugador1(a), jugador2(b), winner_id(winnerId), game_time(g_time) {}

void GameOverState::init(){
    fondo = ResourceManager::getInstance().GetTexture(GetAssetPath("fondo-juego.png"));
    poppins = ResourceManager::getInstance().GetFont(GetAssetPath("Poppins-Bold.ttf"));
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
    const float spacing = 2.0f;

    Color winnerColor = (winner_id == 1) ? SKYBLUE : RED;
    Color panelColor = Fade(DARKGRAY, 0.85f);

    DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.6f));
    DrawCircleGradient(centerX, centerY, 260.0f, Fade(winnerColor, 0.45f), Fade(BLACK, 0.0f));

    const char* title = _("GAME OVER");
    const int titleSize = 52.0f;
    Vector2 titleMeas = MeasureTextEx(poppins, title, titleSize, spacing);

    string winnerText = string(_("Ganador: ")) + (winner_id == 1 ? jugador1.nombre : jugador2.nombre);
    const int winnerSize = 42.0f;
    Vector2 winnerMeas = MeasureTextEx(poppins, winnerText.c_str(), winnerSize, spacing);

    const char* subText = _("¡Enhorabuena!");
    const int subSize = 28.0f;
    Vector2 subMeas = MeasureTextEx(poppins, subText, subSize, spacing);

    string gameTimeText = string(_("Tiempo de juego: ")) + to_string(game_time/60) + _("m ") + to_string(game_time%60) + _("s");
    const int gameTimeSize = 22.0f;
    Vector2 gameTimeMeas = MeasureTextEx(poppins, gameTimeText.c_str(), gameTimeSize, spacing);

    const char* restartHint = _("Pulsa ESPACIO para jugar de nuevo");
    const char* exitHint = _("Pulsa ESCAPE para volver al inicio");
    const int hintSize = 22.0f;
    Vector2 hintMeas = MeasureTextEx(poppins, restartHint, hintSize, spacing);  

    float maxContentWidth = titleMeas.x;
    maxContentWidth = max(maxContentWidth, winnerMeas.x);
    maxContentWidth = max(maxContentWidth, subMeas.x);
    maxContentWidth = max(maxContentWidth, hintMeas.x);

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
    DrawTextEx(poppins, title, { panelCenterX - titleMeas.x / 2.0f, currentY }, (float)titleSize, spacing, RAYWHITE);
    currentY += titleSize + spacingTitleWinner;
    DrawTextEx(poppins, winnerText.c_str(), { panelCenterX - winnerMeas.x / 2.0f, currentY }, (float)winnerSize, spacing, winnerColor);
    currentY += winnerSize + spacingWinnerSub;
    DrawTextEx(poppins, subText, { panelCenterX - subMeas.x / 2.0f, currentY }, (float)subSize, spacing, RAYWHITE);
    currentY += subSize + spacingSubHint;
    DrawTextEx(poppins, gameTimeText.c_str(), { panelCenterX - gameTimeMeas.x / 2.0f, currentY }, (float)gameTimeSize, spacing, LIGHTGRAY);
    currentY += gameTimeSize + 12;
    DrawTextEx(poppins, restartHint, { panelCenterX - hintMeas.x / 2.0f, currentY }, (float)hintSize, spacing, LIGHTGRAY);
    currentY += hintSize + 12;
    Vector2 exitMeas = MeasureTextEx(poppins, exitHint, (float)hintSize, spacing);
    DrawTextEx(poppins, exitHint, { panelCenterX - exitMeas.x / 2.0f, currentY }, (float)hintSize, spacing, LIGHTGRAY);
    EndDrawing();
}

void GameOverState::pause(){

}

void GameOverState::resume(){
    
}
