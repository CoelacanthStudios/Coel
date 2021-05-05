#include <coel/window/core.hpp>
#include <thread>
#include <iostream>

using namespace std::chrono_literals;

int main() {
    coel::Window window(coel::WindowConfig{
        .size{800, 600},
        .options{
            .resizable = true,
            .system_decorations = false,
        },
    });

    bool should_close = false;
    std::size_t resize_count = 0;

    while (true) {
        while (coel::WindowEvent event = window.poll_event()) {
            switch (event.type) {
            case coel::WindowEventType::Close:
                should_close = true;
                break;
            case coel::WindowEventType::Resize:
                std::cout << "resized! (" << resize_count << ") size {" << window.get_width() << ", " << window.get_height() << "}\n";
                resize_count++;
                break;
            }
        }

        if (should_close) {
            break;
        }

        std::this_thread::sleep_for(5ms);
    }
}
