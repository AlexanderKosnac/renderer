#include <X11/Xlib.h>
#include <iostream>

#include "math.h"
#include "modelling/triangle.h"
#include "modelling/mesh.h"
#include "modelling/camera.h"
#include "modelling/scene.h"
#include "display/x11display.h"
#include "display/callbacktypes.h"

int main() {
    modelling::Camera camera(60.0f, 3.0f, 0.1f, 100.0f, math::vec3(0.0f, 0.0f, 0.0f), math::vec3(0.0f, 1.0f, 0.0f), math::vec3(0.0f, 0.0f, 1.0f));

    modelling::Triangle t1;
    t1.setVertex(0, math::vec3(0.0f, 0.0f, 0.0f), math::vec3(255.0f, 255.0f, 255.0f));
    t1.setVertex(1, math::vec3(0.0f, 1.0f, 0.0f), math::vec3(255.0f, 255.0f, 255.0f));
    t1.setVertex(2, math::vec3(0.0f, 0.0f, 1.0f), math::vec3(255.0f, 255.0f, 255.0f));

    modelling::Mesh mesh;
    mesh.addTriangle(t1);

    Scene scene(camera);
    scene.addMesh(mesh);

    int a = 5;

    auto onExpose = [a](XEvent& event) mutable {
        std::cout << "Expose" << std::endl;
    };

    auto onKeyPress = [camera](XEvent& event) mutable {
        math::vec3& pos = camera.getPos();
        math::vec3 axis;
        switch (event.xkey.keycode) {
            case 25: // W
                axis = camera.normedW();
                pos.x += axis.x;
                pos.y += axis.y;
                pos.z += axis.z;
                break;
            case 38: // A
                axis = camera.normedU();
                pos.x -= axis.x;
                pos.y -= axis.y;
                pos.z -= axis.z;
                break;
            case 39: // S
                axis = camera.normedW();
                pos.x -= axis.x;
                pos.y -= axis.y;
                pos.z -= axis.z;
                break;
            case 40: // D
                axis = camera.normedU();
                pos.x += axis.x;
                pos.y += axis.y;
                pos.z += axis.z;
                break;
        }
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

    DisplayX11 display(1024, 512);
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
