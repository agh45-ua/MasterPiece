#pragma once
#include "GameState.hpp"
#include <string>
#include <optional>

#include "../core/ControlBindings.hpp"

extern "C" {
    #include <raylib.h>
}

class ControlesState : public GameState {
    public:
        ControlesState() = default;
        ~ControlesState() = default;

        void init() override;
        void handleInput() override;
        void update(float deltaTime) override;
        void render() override;

        void pause() override;
        void resume() override;

    private:
        Rectangle getBackButtonBounds() const;
        Rectangle getPanelBounds() const;
        Rectangle getActionBounds(ControlAction action) const;
        void startRebinding(ControlAction action);
        void processRebindingInput();
        void cancelRebinding();

        Texture2D fondo;
        Font poppins;
        std::optional<ControlAction> editingAction;
        std::string toastMessage;
        float toastTimer = 0.0f;
};
