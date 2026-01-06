#pragma once
#include "GameState.hpp"
#include <vector>
#include <string>
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
        struct ControlItem {
            std::string title;
            std::string detail;
        };

        Rectangle getBackButtonBounds() const;

        Texture2D fondo;
        Font poppins;
        std::vector<ControlItem> common_controls;
        std::vector<ControlItem> player1_controls;
        std::vector<ControlItem> player2_controls;
};
