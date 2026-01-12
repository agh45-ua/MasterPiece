#pragma once

#include <array>
#include <string>
#include <libintl.h>
#define _(STRING) gettext(STRING)
extern "C" {
#include <raylib.h>
}

enum class InputBindingType {
    Keyboard,
    Mouse
};

struct InputBinding {
    InputBindingType type = InputBindingType::Keyboard;
    int code = KEY_NULL;

    static InputBinding Keyboard(KeyboardKey key);
    static InputBinding Mouse(MouseButton button);

    [[nodiscard]] std::string toDisplayString() const;
    [[nodiscard]] bool isPressed() const;
    [[nodiscard]] bool isDown() const;
};

enum class ControlAction {
    MoveLeft = 0,
    MoveRight,
    FixShot,
    EndTurnP1,
    EndTurnP2,
    Count
};

class ControlBindings {
  public:
    static ControlBindings &Instance();

    const InputBinding &get(ControlAction action) const;
    InputBinding &get(ControlAction action);
    void set(ControlAction action, const InputBinding &binding);
    void resetDefaults();

    static const char *GetActionLabel(ControlAction action);
    bool fixShotUsesMouseFallback() const;

    static InputBinding CreateDefault(ControlAction action);

  private:
    ControlBindings();

    std::array<InputBinding, static_cast<size_t>(ControlAction::Count)> bindings;
    bool fixShotMouseFallback = true;
};

std::string DescribeKeyboardKey(int key);
std::string DescribeMouseButton(int button);
