#include "ControlBindings.hpp"

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
        return "Movimiento izquierda";
    case ControlAction::MoveRight:
        return "Movimiento derecha";
    case ControlAction::FixShot:
        return "Fijar lanzamiento";
    case ControlAction::EndTurnP1:
        return "Finalizar turno J1";
    case ControlAction::EndTurnP2:
        return "Finalizar turno J2";
    case ControlAction::Count:
        break;
    }
    return "";
}

bool ControlBindings::fixShotUsesMouseFallback() const {
    return fixShotMouseFallback;
}

std::string DescribeKeyboardKey(int key) {
    if (key == KEY_NULL) {
        return "Sin asignar";
    }

    const char *name = GetKeyName(key);
    if (name && name[0] != '\0') {
        std::string result(name);
        if (!result.empty() && result[0] >= 'a' && result[0] <= 'z') {
            result[0] = result[0] - 'a' + 'A';
        }
        return result;
    }

    switch (key) {
    case KEY_SPACE:
        return "Espacio";
    case KEY_ENTER:
        return "Enter";
    case KEY_TAB:
        return "Tab";
    case KEY_LEFT_CONTROL:
        return "Ctrl izq";
    case KEY_RIGHT_CONTROL:
        return "Ctrl der";
    case KEY_LEFT_SHIFT:
        return "Shift izq";
    case KEY_RIGHT_SHIFT:
        return "Shift der";
    case KEY_LEFT_ALT:
        return "Alt izq";
    case KEY_RIGHT_ALT:
        return "Alt der";
    case KEY_BACKSPACE:
        return "Backspace";
    case KEY_DELETE:
        return "Delete";
    case KEY_UP:
        return "Flecha arriba";
    case KEY_DOWN:
        return "Flecha abajo";
    case KEY_LEFT:
        return "Flecha izq";
    case KEY_RIGHT:
        return "Flecha der";
    default:
        break;
    }

    if (key >= KEY_F1 && key <= KEY_F12) {
        return "F" + std::to_string(1 + key - KEY_F1);
    }

    return "Tecla " + std::to_string(key);
}

std::string DescribeMouseButton(int button) {
    switch (button) {
    case MOUSE_BUTTON_LEFT:
        return "Click izq";
    case MOUSE_BUTTON_RIGHT:
        return "Click der";
    case MOUSE_BUTTON_MIDDLE:
        return "Click medio";
    case MOUSE_BUTTON_SIDE:
        return "Mouse lateral";
    case MOUSE_BUTTON_EXTRA:
        return "Mouse extra";
    case MOUSE_BUTTON_FORWARD:
        return "Mouse adelante";
    case MOUSE_BUTTON_BACK:
        return "Mouse atrás";
    default:
        break;
    }
    return "Boton " + std::to_string(button);
}
