#include <cassert>
#include <memory>
#include "core/StateMachine.hpp"
#include "core/GameState.hpp"

// Estado de prueba que simula el comportamiento de los estados reales
class DummyState : public GameState {
public:
    bool initialized = false;
    bool updated = false;
    bool rendered = false;
    bool paused = false;
    bool resumed = false;

    void init() override { initialized = true; }
    void handleInput() override {}
    void update(float) override { updated = true; }
    void render() override { rendered = true; }
    void pause() override { paused = true; }
    void resume() override { resumed = true; }
};

int main() {
    StateMachine sm;
    float dt = 0.016f;

    // TEST 1: Añadir primer estado e inicialización
    auto state1 = std::make_unique<DummyState>();
    DummyState* raw1 = state1.get();
    sm.add_state(std::move(state1), true);
    sm.handle_state_changes(dt);
    assert(raw1->initialized == true);

    // TEST 2: Funcionamiento de Update
    sm.getCurrentState()->update(dt);
    assert(raw1->updated == true);

    // TEST 3: Funcionamiento de Render
    sm.getCurrentState()->render();
    assert(raw1->rendered == true);

    // TEST 4: Apilado y Pausa (Push)
    auto state2 = std::make_unique<DummyState>();
    DummyState* raw2 = state2.get();
    sm.add_state(std::move(state2), false);
    sm.handle_state_changes(dt);
    assert(raw1->paused == true);

    // TEST 5: Desapilado y Reanudación (Pop)
    sm.remove_state(false);
    sm.handle_state_changes(dt);
    assert(raw1->resumed == true);

    // TEST 6: Reset de DeltaTime tras cambio de estado
    dt = 0.5f;
    sm.add_state(std::make_unique<DummyState>(), true);
    sm.handle_state_changes(dt);
    assert(dt == 0.0f);

    return 0;
}