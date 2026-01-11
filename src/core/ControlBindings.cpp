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

std::string DescribeKeyboardKey(int key) {
    if (key == KEY_NULL) {
        return _("Sin asignar");
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
