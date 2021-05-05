#pragma once

#include <array>
#include <coel/window/native.hpp>
#include <coel/window/event.hpp>

namespace coel {
    struct WindowConfig {
        std::array<int, 2> size;
        struct {
            bool resizable : 1 = false, system_decorations = true;
        } options;
    };

    class Window {
        NativeWindowState native;
        std::array<int, 2> dim;

      public:
        Window() = delete;
        Window(const WindowConfig &config);
        ~Window();

        Window(const Window &) = delete;
        Window &operator=(const Window &) = delete;

        Window(Window &&);
        Window &operator=(Window &&);

        [[nodiscard]] WindowEvent poll_event();
        [[nodiscard]] constexpr int get_width() const { return dim[0]; }
        [[nodiscard]] constexpr int get_height() const { return dim[1]; }
    };
} // namespace coel
