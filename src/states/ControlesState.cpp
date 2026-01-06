#include "ControlesState.hpp"
#include "InicioState.hpp"
#include "StateMachine.hpp"
#include "../core/config.h"
#include <memory>
#include <sstream>
#include <array>

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
    editingAction.reset();
    toastMessage.clear();
    toastTimer = 0.0f;
}

void ControlesState::handleInput(){
    if (editingAction.has_value()) {
        processRebindingInput();
        return;
    }

    Rectangle backButton = getBackButtonBounds();
    Vector2 mouse = GetMousePosition();
    bool click = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    if (click) {
        if (CheckCollisionPointRec(mouse, backButton)) {
            this->state_machine->add_state(std::make_unique<InicioState>(), true);
            return;
        }

        static const std::array<ControlAction, static_cast<size_t>(ControlAction::Count)> actions = {
            ControlAction::MoveLeft,
            ControlAction::MoveRight,
            ControlAction::FixShot,
            ControlAction::EndTurnP1,
            ControlAction::EndTurnP2
        };
        for (ControlAction action : actions) {
            if (CheckCollisionPointRec(mouse, getActionBounds(action))) {
                startRebinding(action);
                return;
            }
        }
    }
}

void ControlesState::update(float deltaTime){
    if (toastTimer > 0.0f) {
        toastTimer -= deltaTime;
        if (toastTimer <= 0.0f) {
            toastTimer = 0.0f;
            toastMessage.clear();
        }
    }
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

    Rectangle panel = getPanelBounds();
    DrawRectangleRounded(panel, 0.12f, 8, Fade(BLACK, 0.65f));
    DrawRectangleRoundedLines(panel, 0.12f, 8, Fade(RAYWHITE, 0.6f));

    const float titleSize = 36.0f;
    const float textSize = 20.0f;
    Vector2 titlePos = {panel.x + 50.0f, panel.y + 40.0f};
    DrawTextEx(poppins, "Configuracion de teclas", titlePos, titleSize, 2, RAYWHITE);

    float columnsTop = titlePos.y + titleSize + 30.0f;
    float leftColumnX = panel.x + 40.0f;
    float rightColumnX = panel.x + panel.width / 2.0f + 40.0f;
    DrawTextEx(poppins, "Controles comunes", {leftColumnX, columnsTop}, textSize + 4.0f, 2, GOLD);
    DrawTextEx(poppins, "Controles por jugador", {rightColumnX, columnsTop}, textSize + 4.0f, 2, PINK);

    ControlBindings &bindings = ControlBindings::Instance();
    Vector2 currentMouse = GetMousePosition();
    auto drawActionRow = [&](ControlAction action) {
        Rectangle bounds = getActionBounds(action);
        bool hover = CheckCollisionPointRec(currentMouse, bounds);
        bool editing = editingAction.has_value() && editingAction.value() == action;
        Color base = Fade(BLACK, editing ? 0.7f : (hover ? 0.55f : 0.4f));
        DrawRectangleRounded(bounds, 0.18f, 6, base);

        const char *label = ControlBindings::GetActionLabel(action);
        DrawTextEx(
            poppins,
            label,
            {bounds.x + 18.0f, bounds.y + 14.0f},
            textSize,
            2.0f,
            RAYWHITE
        );

        std::string bindingText = bindings.get(action).toDisplayString();
        Color bindingColor = editing ? YELLOW : SKYBLUE;
        Vector2 size = MeasureTextEx(poppins, bindingText.c_str(), textSize, 2.0f);
        DrawTextEx(
            poppins,
            bindingText.c_str(),
            {bounds.x + bounds.width - size.x - 18.0f, bounds.y + 14.0f},
            textSize,
            2.0f,
            bindingColor
        );
    };

    drawActionRow(ControlAction::MoveLeft);
    drawActionRow(ControlAction::MoveRight);
    drawActionRow(ControlAction::FixShot);
    drawActionRow(ControlAction::EndTurnP1);
    drawActionRow(ControlAction::EndTurnP2);

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

    const Rectangle lastLeft = getActionBounds(ControlAction::FixShot);
    const Rectangle lastRight = getActionBounds(ControlAction::EndTurnP2);
    float helperY = std::max(lastLeft.y + lastLeft.height, lastRight.y + lastRight.height) + 30.0f;
    helperY = std::min(helperY, panel.y + panel.height - 120.0f);

    std::string helperText;
    if (editingAction.has_value()) {
        helperText = "Pulsa una tecla o boton del raton para asignarla (ESC para cancelar)";
    } else {
        helperText = "Haz clic sobre un control para reasignarlo.";
    }
    DrawWrappedText(
        poppins,
        helperText,
        {panel.x + 40.0f, helperY},
        textSize - 2.0f,
        2.0f,
        panel.width - 80.0f,
        LIGHTGRAY
    );

    if (toastTimer > 0.0f && !toastMessage.empty()) {
        float toastY = helperY + 40.0f;
        toastY = std::min(toastY, panel.y + panel.height - 60.0f);
        DrawTextEx(
            poppins,
            toastMessage.c_str(),
            {panel.x + 40.0f, toastY},
            textSize - 2.0f,
            2.0f,
            SKYBLUE
        );
    }

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

Rectangle ControlesState::getPanelBounds() const {
    const float width = 720.0f;
    const float height = 520.0f;
    return {
        (float)(GetScreenWidth() / 2.0f - width / 2.0f),
        (float)(GetScreenHeight() / 2.0f - height / 2.0f),
        width,
        height
    };
}

Rectangle ControlesState::getActionBounds(ControlAction action) const {
    Rectangle panel = getPanelBounds();
    const float columnWidth = panel.width / 2.0f - 80.0f;
    const float leftX = panel.x + 40.0f;
    const float rightX = panel.x + panel.width / 2.0f + 40.0f;
    const float startY = panel.y + 150.0f;
    const float rowHeight = 64.0f;
    const float gap = 18.0f;

    auto findIndex = [](const ControlAction *values, size_t count, ControlAction target) -> int {
        for (size_t idx = 0; idx < count; ++idx) {
            if (values[idx] == target) {
                return static_cast<int>(idx);
            }
        }
        return -1;
    };

    static constexpr ControlAction leftColumn[] = {
        ControlAction::MoveLeft,
        ControlAction::MoveRight,
        ControlAction::FixShot
    };
    static constexpr ControlAction rightColumn[] = {
        ControlAction::EndTurnP1,
        ControlAction::EndTurnP2
    };

    int index = findIndex(leftColumn, sizeof(leftColumn) / sizeof(ControlAction), action);
    if (index >= 0) {
        return {leftX, startY + index * (rowHeight + gap), columnWidth, rowHeight};
    }

    index = findIndex(rightColumn, sizeof(rightColumn) / sizeof(ControlAction), action);
    if (index >= 0) {
        return {rightX, startY + index * (rowHeight + gap), columnWidth, rowHeight};
    }

    return {panel.x, panel.y, 0.0f, 0.0f};
}

void ControlesState::startRebinding(ControlAction action) {
    editingAction = action;
}

void ControlesState::processRebindingInput() {
    if (!editingAction.has_value()) {
        return;
    }

    int key = GetKeyPressed();
    if (key != KEY_NULL) {
        if (key == KEY_ESCAPE) {
            cancelRebinding();
            return;
        }

        ControlBindings::Instance().set(editingAction.value(), InputBinding::Keyboard(static_cast<KeyboardKey>(key)));
        toastMessage = std::string(ControlBindings::GetActionLabel(editingAction.value())) + " -> " + DescribeKeyboardKey(key);
        toastTimer = 3.0f;
        editingAction.reset();
        return;
    }

    static const MouseButton mouseButtons[] = {
        MOUSE_BUTTON_LEFT,
        MOUSE_BUTTON_RIGHT,
        MOUSE_BUTTON_MIDDLE,
        MOUSE_BUTTON_SIDE,
        MOUSE_BUTTON_EXTRA,
        MOUSE_BUTTON_FORWARD,
        MOUSE_BUTTON_BACK
    };

    for (MouseButton button : mouseButtons) {
        if (IsMouseButtonPressed(button)) {
            ControlBindings::Instance().set(editingAction.value(), InputBinding::Mouse(button));
            toastMessage = std::string(ControlBindings::GetActionLabel(editingAction.value())) + " -> " + DescribeMouseButton(button);
            toastTimer = 3.0f;
            editingAction.reset();
            return;
        }
    }
}

void ControlesState::cancelRebinding() {
    editingAction.reset();
}
