#include <X11/Xlib.h>
#include <iostream>

#include "math.h"
#include "modelling/mesh.h"
#include "modelling/triangle.h"
#include "display/x11display.h"
#include "display/callbacktypes.h"

int main() {
    int a = 5;

    auto onExpose = [a](XEvent& event) mutable {
        std::cout << "Expose" << std::endl;
    };

    auto onKeyPress = [a](XEvent& event) mutable {
        std::cout << "Key pressed: " << XLookupKeysym(&event.xkey, 0) << std::endl;
    };

    auto onKeyRelease = [a](XEvent& event) mutable {
        std::cout << "Key released: " << XLookupKeysym(&event.xkey, 0) << std::endl;
    };

    auto onButtonPress = [a](XEvent& event) mutable {
        std::cout << "Mouse Button " << event.xbutton.button << " Pressed at (" << event.xbutton.x  << ", " << event.xbutton.y << ")" << std::endl;
    };

    auto onButtonRelease = [a](XEvent& event) mutable {
        std::cout << "Mouse Button " << event.xbutton.button << " Released at (" << event.xbutton.x << ", " << event.xbutton.y << ")" << std::endl;
    };

    auto onMouseMotion = [a](XEvent& event) mutable {
        std::cout << "Mouse Moved to (" << event.xmotion.x << ", " << event.xmotion.y << ")" << std::endl;
    };

    DisplayX11 display(500, 500);
    display.addListener(EXPOSE, onExpose);
    display.addListener(KEY_PRESS, onKeyPress);
    display.addListener(KEY_RELEASE, onKeyRelease);
    display.addListener(BUTTON_PRESS, onButtonPress);
    display.addListener(BUTTON_RELEASE, onButtonRelease);
    display.addListener(MOUSE_MOTION, onMouseMotion);

    bool running = true;
    while (running) {
        std::cout << "tick" << std::endl;
        display.update();
    }
}
