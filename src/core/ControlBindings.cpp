#include "ControlBindings.hpp"
extern "C" {
    #include <raylib.h>
}

InputBinding ControlBindings::CreateDefault(ControlAction action) {
    switch (action) {
    case ControlAction::MoveLeft:
        return InputBinding::Keyboard(KEY_A);
    case ControlAction::MoveRight:
        return InputBinding::Keyboard(KEY_D);
    case ControlAction::FixShot:
        return InputBinding::Mouse(MOUSE_BUTTON_LEFT);
    case ControlAction::EndTurnP1:
        return InputBinding::Keyboard(KEY_ENTER);
    case ControlAction::EndTurnP2:
        return InputBinding::Keyboard(KEY_Q);
    case ControlAction::Count:
        break;
    }
    return InputBinding::Keyboard(KEY_NULL);
}

InputBinding InputBinding::Keyboard(KeyboardKey key) {
    InputBinding binding;
    binding.type = InputBindingType::Keyboard;
    binding.code = key;
    return binding;
}

InputBinding InputBinding::Mouse(MouseButton button) {
    InputBinding binding;
    binding.type = InputBindingType::Mouse;
    binding.code = button;
    return binding;
}

std::string InputBinding::toDisplayString() const {
    if (type == InputBindingType::Keyboard) {
        return DescribeKeyboardKey(code);
    }
    return DescribeMouseButton(code);
}

bool InputBinding::isPressed() const {
    if (type == InputBindingType::Keyboard) {
        return IsKeyPressed(static_cast<KeyboardKey>(code));
    }
    return IsMouseButtonPressed(static_cast<MouseButton>(code));
}

bool InputBinding::isDown() const {
    if (type == InputBindingType::Keyboard) {
        return IsKeyDown(static_cast<KeyboardKey>(code));
    }
    return IsMouseButtonDown(static_cast<MouseButton>(code));
}

ControlBindings::ControlBindings() {
    resetDefaults();
}

ControlBindings &ControlBindings::Instance() {
    static ControlBindings instance;
    return instance;
}

const InputBinding &ControlBindings::get(ControlAction action) const {
    return bindings.at(static_cast<size_t>(action));
}

InputBinding &ControlBindings::get(ControlAction action) {
    return bindings.at(static_cast<size_t>(action));
}

void ControlBindings::set(ControlAction action, const InputBinding &binding) {
    bindings.at(static_cast<size_t>(action)) = binding;
    if (action == ControlAction::FixShot) {
        fixShotMouseFallback =
            (binding.type == InputBindingType::Keyboard && binding.code == KEY_SPACE);
    }
}

void ControlBindings::resetDefaults() {
    for (size_t idx = 0; idx < bindings.size(); ++idx) {
        bindings[idx] = CreateDefault(static_cast<ControlAction>(idx));
    }
    fixShotMouseFallback = true;
}

const char *ControlBindings::GetActionLabel(ControlAction action) {
    switch (action) {
    case ControlAction::MoveLeft:
        return _("Movimiento izquierda");
    case ControlAction::MoveRight:
        return _("Movimiento derecha");
    case ControlAction::FixShot:
        return _("Fijar lanzamiento");
    case ControlAction::EndTurnP1:
        return _("Finalizar turno J1");
    case ControlAction::EndTurnP2:
        return _("Finalizar turno J2");
    case ControlAction::Count:
        break;
    }
    return "";
}

bool ControlBindings::fixShotUsesMouseFallback() const {
    return fixShotMouseFallback;
}

// Implementación de GetKeyNameOwn si no está disponible en raylib
static const char* GetKeyNameOwn(int key) {
    switch (key) {
    case KEY_NULL: return "NULL";
    case KEY_APOSTROPHE: return "'";
    case KEY_COMMA: return ",";
    case KEY_MINUS: return "-";
    case KEY_PERIOD: return ".";
    case KEY_SLASH: return "/";
    case KEY_ZERO: return "0";
    case KEY_ONE: return "1";
    case KEY_TWO: return "2";
    case KEY_THREE: return "3";
    case KEY_FOUR: return "4";
    case KEY_FIVE: return "5";
    case KEY_SIX: return "6";
    case KEY_SEVEN: return "7";
    case KEY_EIGHT: return "8";
    case KEY_NINE: return "9";
    case KEY_SEMICOLON: return ";";
    case KEY_EQUAL: return "=";
    case KEY_A: return "A";
    case KEY_B: return "B";
    case KEY_C: return "C";
    case KEY_D: return "D";
    case KEY_E: return "E";
    case KEY_F: return "F";
    case KEY_G: return "G";
    case KEY_H: return "H";
    case KEY_I: return "I";
    case KEY_J: return "J";
    case KEY_K: return "K";
    case KEY_L: return "L";
    case KEY_M: return "M";
    case KEY_N: return "N";
    case KEY_O: return "O";
    case KEY_P: return "P";
    case KEY_Q: return "Q";
    case KEY_R: return "R";
    case KEY_S: return "S";
    case KEY_T: return "T";
    case KEY_U: return "U";
    case KEY_V: return "V";
    case KEY_W: return "W";
    case KEY_X: return "X";
    case KEY_Y: return "Y";
    case KEY_Z: return "Z";
    case KEY_LEFT_BRACKET: return "[";
    case KEY_BACKSLASH: return "\\";
    case KEY_RIGHT_BRACKET: return "]";
    case KEY_GRAVE: return "`";
    case KEY_SPACE: return "Space";
    case KEY_ESCAPE: return "Esc";
    case KEY_ENTER: return "Enter";
    case KEY_TAB: return "Tab";
    case KEY_BACKSPACE: return "Backspace";
    case KEY_INSERT: return "Ins";
    case KEY_DELETE: return "Del";
    case KEY_RIGHT: return "Right";
    case KEY_LEFT: return "Left";
    case KEY_DOWN: return "Down";
    case KEY_UP: return "Up";
    case KEY_PAGE_UP: return "PgUp";
    case KEY_PAGE_DOWN: return "PgDn";
    case KEY_HOME: return "Home";
    case KEY_END: return "End";
    case KEY_CAPS_LOCK: return "CapsLock";
    case KEY_SCROLL_LOCK: return "ScrollLock";
    case KEY_NUM_LOCK: return "NumLock";
    case KEY_PRINT_SCREEN: return "PrtScn";
    case KEY_PAUSE: return "Pause";
    case KEY_F1: return "F1";
    case KEY_F2: return "F2";
    case KEY_F3: return "F3";
    case KEY_F4: return "F4";
    case KEY_F5: return "F5";
    case KEY_F6: return "F6";
    case KEY_F7: return "F7";
    case KEY_F8: return "F8";
    case KEY_F9: return "F9";
    case KEY_F10: return "F10";
    case KEY_F11: return "F11";
    case KEY_F12: return "F12";
    case KEY_LEFT_SHIFT: return "Shift L";
    case KEY_LEFT_CONTROL: return "Ctrl L";
    case KEY_LEFT_ALT: return "Alt L";
    case KEY_LEFT_SUPER: return "Super L";
    case KEY_RIGHT_SHIFT: return "Shift R";
    case KEY_RIGHT_CONTROL: return "Ctrl R";
    case KEY_RIGHT_ALT: return "Alt R";
    case KEY_RIGHT_SUPER: return "Super R";
    case KEY_KB_MENU: return "Menu";
    case KEY_KP_0: return "Kp0";
    case KEY_KP_1: return "Kp1";
    case KEY_KP_2: return "Kp2";
    case KEY_KP_3: return "Kp3";
    case KEY_KP_4: return "Kp4";
    case KEY_KP_5: return "Kp5";
    case KEY_KP_6: return "Kp6";
    case KEY_KP_7: return "Kp7";
    case KEY_KP_8: return "Kp8";
    case KEY_KP_9: return "Kp9";
    case KEY_KP_DECIMAL: return "Kp.";
    case KEY_KP_DIVIDE: return "Kp/";
    case KEY_KP_MULTIPLY: return "Kp*";
    case KEY_KP_SUBTRACT: return "Kp-";
    case KEY_KP_ADD: return "Kp+";
    case KEY_KP_ENTER: return "KpEnter";
    case KEY_KP_EQUAL: return "Kp=";
    default: return "";
    }
}

std::string DescribeKeyboardKey(int key) {
    if (key == KEY_NULL) {
        return _("Sin asignar");
    }

    const char *name = GetKeyNameOwn(key);
    if (name && name[0] != '\0') {
        std::string result(name);
        if (!result.empty() && result[0] >= 'a' && result[0] <= 'z') {
            result[0] = result[0] - 'a' + 'A';
        }
        return result;
    }

    switch (key) {
    case KEY_SPACE:
        return _("Espacio");
    case KEY_ENTER:
        return _("Intro");
    case KEY_TAB:
        return _("Tab");
    case KEY_LEFT_CONTROL:
        return _("Ctrl izq");
    case KEY_RIGHT_CONTROL:
        return _("Ctrl der");
    case KEY_LEFT_SHIFT:
        return _("Shift izq");
    case KEY_RIGHT_SHIFT:
        return _("Shift der");
    case KEY_LEFT_ALT:
        return _("Alt izq");
    case KEY_RIGHT_ALT:
        return _("Alt der");
    case KEY_BACKSPACE:
        return _("Backspace");
    case KEY_DELETE:
        return _("Delete");
    case KEY_UP:
        return _("Flecha arriba");
    case KEY_DOWN:
        return _("Flecha abajo");
    case KEY_LEFT:
        return _("Flecha izq");
    case KEY_RIGHT:
        return _("Flecha der");
    default:
        break;
    }

    if (key >= KEY_F1 && key <= KEY_F12) {
        return "F" + std::to_string(1 + key - KEY_F1);
    }

    return std::string(_("Tecla ")) + std::to_string(key);
}

std::string DescribeMouseButton(int button) {
    switch (button) {
    case MOUSE_BUTTON_LEFT:
        return _("Click izq");
    case MOUSE_BUTTON_RIGHT:
        return _("Click der");
    case MOUSE_BUTTON_MIDDLE:
        return _("Click medio");
    case MOUSE_BUTTON_SIDE:
        return _("Mouse lateral");
    case MOUSE_BUTTON_EXTRA:
        return _("Mouse extra");
    case MOUSE_BUTTON_FORWARD:
        return _("Mouse adelante");
    case MOUSE_BUTTON_BACK:
        return _("Mouse atrás");
    default:
        break;
    }
    return std::string(_("Boton ")) + std::to_string(button);
}
