#include "ControlesState.hpp"
#include "InicioState.hpp"
#include "StateMachine.hpp"
#include "../core/config.h"
#include <memory>
#include <sstream>

float DrawWrappedText(Font font, const std::string& text, Vector2 position, float fontSize, float spacing, float maxWidth, Color color) {
    std::istringstream stream(text);
    std::string word;
    std::string line;
    float heightUsed = 0.0f;

    while (stream >> word) {
        std::string testLine = line.empty() ? word : line + " " + word;
        float testWidth = MeasureTextEx(font, testLine.c_str(), fontSize, spacing).x;
        if (testWidth > maxWidth && !line.empty()) {
            DrawTextEx(font, line.c_str(), {position.x, position.y + heightUsed}, fontSize, spacing, color);
            heightUsed += fontSize + 4.0f;
            line = word;
        } else {
            line = testLine;
        }
    }

    if (!line.empty()) {
        DrawTextEx(font, line.c_str(), {position.x, position.y + heightUsed}, fontSize, spacing, color);
        heightUsed += fontSize;
    }

    return heightUsed;
}

void ControlesState::init(){
    fondo = LoadTexture(GetAssetPath("fondo-inicio.png").c_str());
    poppins = LoadFontEx(GetAssetPath("Poppins-Bold.ttf").c_str(), 64, 0, 0);
    common_controls = {
        {"Movimiento", "Ambos jugadores se desplazan con A y D (max 200px por turno)"},
        {"Fijacion del disparo", "Espacio o clic izquierdo para fijar el disparo"},
    };
    player1_controls = {
        {"Terminar preparacion", "ENTER cede el turno al jugador 2"},
    };
    player2_controls = {
        {"Resolver turno", "Q inicia la cuenta atras y resuelve los disparos"},
    };
}

void ControlesState::handleInput(){
    Rectangle backButton = getBackButtonBounds();

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Vector2 mouse = GetMousePosition();
        if (CheckCollisionPointRec(mouse, backButton)) {
            this->state_machine->add_state(std::make_unique<InicioState>(), true);
        }
    }

    if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_ENTER)) {
        this->state_machine->add_state(std::make_unique<InicioState>(), true);
    }
}

void ControlesState::update(float deltaTime){

}

void ControlesState::render(){
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
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.8f));

    Rectangle panel = {
        (float)(GetScreenWidth()/2 - 330),
        (float)(GetScreenHeight()/2 - 220),
        660.0f,
        440.0f
    };
    DrawRectangleRounded(panel, 0.12f, 8, Fade(BLACK, 0.65f));
    DrawRectangleRoundedLines(panel, 0.12f, 8, Fade(RAYWHITE, 0.6f));

    const float titleSize = 36.0f;
    const float textSize = 20.0f;
    Vector2 titlePos = {panel.x + 50.0f, panel.y + 40.0f};
    DrawTextEx(poppins, "Configuracion de teclas", titlePos, titleSize, 2, RAYWHITE);

    float currentY = titlePos.y + titleSize + 20.0f;
    DrawTextEx(poppins, "Controles comunes", {panel.x + 40.0f, currentY}, textSize + 4.0f, 2, GOLD);
    currentY += textSize + 16.0f;
    for (const auto& item : common_controls) {
        DrawTextEx(poppins, item.title.c_str(), {panel.x + 40.0f, currentY}, textSize, 2, SKYBLUE);
        currentY += textSize + 8.0f;
        float used = DrawWrappedText(
            poppins,
            item.detail,
            {panel.x + 60.0f, currentY},
            textSize - 2.0f,
            2.0f,
            panel.width - 120.0f,
            RAYWHITE
        );
        currentY += used + 12.0f;
    }
    currentY += 12.0f;

    float columnsTop = currentY;
    float leftColumnX = panel.x + 50.0f;
    float rightColumnX = panel.x + panel.width / 2.0f + 30.0f;
    float columnWidth = panel.width / 2.0f - 80.0f;

    DrawTextEx(poppins, "Jugador 1", {leftColumnX, columnsTop}, textSize + 4.0f, 2, SKYBLUE);
    DrawTextEx(poppins, "Jugador 2", {rightColumnX, columnsTop}, textSize + 4.0f, 2, PINK);

    float leftY = columnsTop + textSize + 10.0f;
    for (const auto& item : player1_controls) {
        DrawTextEx(poppins, item.title.c_str(), {leftColumnX, leftY}, textSize, 2, RAYWHITE);
        leftY += textSize + 6.0f;
        float used = DrawWrappedText(
            poppins,
            item.detail,
            {leftColumnX + 20.0f, leftY},
            textSize - 2.0f,
            2.0f,
            columnWidth,
            LIGHTGRAY
        );
        leftY += used + 10.0f;
    }

    float rightY = columnsTop + textSize + 10.0f;
    for (const auto& item : player2_controls) {
        DrawTextEx(poppins, item.title.c_str(), {rightColumnX, rightY}, textSize, 2, RAYWHITE);
        rightY += textSize + 6.0f;
        float used = DrawWrappedText(
            poppins,
            item.detail,
            {rightColumnX + 20.0f, rightY},
            textSize - 2.0f,
            2.0f,
            columnWidth,
            LIGHTGRAY
        );
        rightY += used + 10.0f;
    }

    Rectangle backButton = getBackButtonBounds();
    Vector2 mouse = GetMousePosition();
    bool hover = CheckCollisionPointRec(mouse, backButton);
    DrawRectangleRounded(backButton, 0.15f, 8, hover ? DARKGREEN : GREEN);

    const char* backText = "Volver";
    Vector2 textSizeVec = MeasureTextEx(poppins, backText, 24.0f, 2.0f);
    DrawTextEx(
        poppins,
        backText,
        {backButton.x + (backButton.width - textSizeVec.x) / 2.0f, backButton.y + (backButton.height - textSizeVec.y) / 2.0f},
        24.0f,
        2.0f,
        RAYWHITE
    );

    EndDrawing();
}

void ControlesState::pause(){

}

void ControlesState::resume(){

}

Rectangle ControlesState::getBackButtonBounds() const {
    const float width = 220.0f;
    const float height = 52.0f;
    return {
        (float)(GetScreenWidth() / 2.0f - width / 2.0f),
        (float)(GetScreenHeight() - 120.0f),
        width,
        height
    };
}
