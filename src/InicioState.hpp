#pragma once
#include "GameState.hpp"
#include <raylib.h>
#include <cmath>
#include <string>
extern "C" {
    #include <raylib.h>
}

using namespace std;

struct player {
    string name;
    int id;
};

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
};