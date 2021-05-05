#include <coel/window/core.hpp>
#include <Windows.h>
#include <bit>

namespace coel {
    struct WindowProcData {
        coel::Window &window;
        coel::WindowEvent event;
    };

    Window::Window(const WindowConfig &config) {
        constexpr auto window_proc = [](HWND win32_window_handle, UINT message_id, WPARAM wp, LPARAM lp) -> LRESULT {
            auto window_proc_data_ptr = std::bit_cast<WindowProcData *>(GetWindowLongPtrA(win32_window_handle, GWLP_USERDATA));
            if (window_proc_data_ptr) {
                auto &data = *window_proc_data_ptr;
                switch (message_id) {
                case WM_SIZE: {
                    data.event.type = coel::WindowEventType::Resize;
                    const int width = LOWORD(lp);
                    const int height = HIWORD(lp);
                    data.event.resize = {width, height};
                    const bool iconified = wp == SIZE_MINIMIZED;
                    const bool maximized = wp == SIZE_MAXIMIZED;
                    data.event.resize.minimized = iconified;
                    data.event.resize.maximized = maximized;
                    data.window.dim = {width, height};

                    // if (_glfw.win32.disabledCursorWindow == window)
                    //     updateClipRect(window);
                    // if (window->win32.iconified != iconified)
                    //     _glfwInputWindowIconify(window, iconified);
                    // if (window->win32.maximized != maximized)
                    //     _glfwInputWindowMaximize(window, maximized);

                    // if (width != window->win32.width || height != window->win32.height) {
                    //     window->win32.width = width;
                    //     window->win32.height = height;
                    //     _glfwInputFramebufferSize(window, width, height);
                    //     _glfwInputWindowSize(window, width, height);
                    // }

                    // if (window->monitor && window->win32.iconified != iconified) {
                    //     if (iconified)
                    //         releaseMonitor(window);
                    //     else {
                    //         acquireMonitor(window);
                    //         fitToMonitor(window);
                    //     }
                    // }
                    return 0;
                }

                break;
                case WM_CLOSE:
                    data.event.type = coel::WindowEventType::Close;
                    break;
                }
            }
            return DefWindowProcA(win32_window_handle, message_id, wp, lp);
        };

        static HINSTANCE win32_application_instance = nullptr;
        if (win32_application_instance == nullptr) {
            win32_application_instance = GetModuleHandleA(nullptr);
            WNDCLASSEXA win32_window_class = {
                .cbSize = sizeof(WNDCLASSEXA),
                .style = CS_HREDRAW | CS_VREDRAW,
                .lpfnWndProc = window_proc,
                .cbClsExtra = 0,
                .cbWndExtra = 0,
                .hInstance = win32_application_instance,
                .hIcon = nullptr,
                .hCursor = nullptr,
                .hbrBackground = nullptr,
                .lpszMenuName = nullptr,
                .lpszClassName = "coel::Window",
                .hIconSm = nullptr,
            };
            RegisterClassExA(&win32_window_class);
        }

        const auto style = WS_OVERLAPPEDWINDOW;

        RECT rect{0, 0, config.size[0], config.size[1]};
        AdjustWindowRect(&rect, style, FALSE);

        native.win32_window_handle = CreateWindowExA(
            0, "coel::Window", "title", style,
            CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left, rect.bottom - rect.top,
            nullptr, nullptr, win32_application_instance, nullptr);

        ShowWindow(static_cast<HWND>(native.win32_window_handle), SW_SHOW);
    }
    Window::~Window() {
        DestroyWindow(static_cast<HWND>(native.win32_window_handle));
    }

    Window::Window(Window &&other) {
        native = other.native;
        other.native.win32_window_handle = nullptr;
    }
    Window &Window::operator=(Window &&other) {
        native = other.native;
        other.native.win32_window_handle = nullptr;
        return *this;
    }

    WindowEvent Window::poll_event() {
        MSG win32_message;
        WindowProcData window_proc_data{
            .window = *this,
            .event = {},
        };
        SetWindowLongPtrA(static_cast<HWND>(native.win32_window_handle), GWLP_USERDATA, std::bit_cast<LONG_PTR>(&window_proc_data));
        if (PeekMessageA(&win32_message, static_cast<HWND>(native.win32_window_handle), 0, 0, PM_REMOVE)) {
            TranslateMessage(&win32_message);
            DispatchMessageA(&win32_message);
            return window_proc_data.event;
        }
        return {};
    }
} // namespace coel
