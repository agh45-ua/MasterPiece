#pragma once
#include "GameState.hpp"
#include <raylib.h>
#include <cmath>
#include <string>
#include <libintl.h>
#define _(STRING) gettext(STRING)
extern "C" {
    #include <raylib.h>
}

using namespace std;

class InicioState: public GameState {
    public:
        InicioState();
        ~InicioState() = default;

        void init() override;
        void handleInput() override;
        void update(float deltaTime) override;
        void render() override;

        void pause();
        void resume();
    private:
        Texture2D fondo;
        Font poppins;
};