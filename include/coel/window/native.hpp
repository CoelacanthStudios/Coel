#pragma once

namespace coel {
#if defined(_WIN32) || defined(_WIN64)
    struct NativeWindowState {
        void *win32_window_handle;
    };
#endif
} // namespace coel
