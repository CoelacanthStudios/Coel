#pragma once

namespace coel {
    enum class WindowEventType {
        None,
        Close,
        Resize,
        MouseMove,
    };

    struct WindowEvent {
        WindowEventType type = WindowEventType::None;
        struct {
            int x, y;
        } mouse_move;
        struct {
            int width, height;
            bool minimized:1 = false, maximized:1 = false;
        } resize;

        constexpr operator bool() const { return type != WindowEventType::None; }
    };
} // namespace coel
