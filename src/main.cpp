#include <X11/Xlib.h>

#include <iostream>

#include "math.h"
#include "modelling/triangle.h"
#include "modelling/mesh.h"
#include "modelling/camera.h"
#include "modelling/scene.h"
#include "rasterizer/rasterizer.h"
#include "display/x11display.h"
#include "display/callbacktypes.h"

int main() {
    modelling::Camera cam(90.0f, 4.0/3.0, 0.1f, 100.0f, math::vec3(0.0f, 0.0f, 0.0f), math::vec3(0.0f, 1.0f, 0.0f), math::vec3(0.0f, 0.0f, 1.0f));

    //modelling::Mesh mesh("objs/axis.obj");
    modelling::Mesh mesh("objs/cube.obj");

    math::vec3 light(0.0f, -1.0f, 0.0f);

    Scene scene(cam, light);
    scene.addMesh(mesh);

    modelling::Camera& camera = scene.getCamera();

    auto onExpose = [&camera](XEvent& event) mutable {
        std::cout << "Expose" << std::endl;
    };

    auto onKeyPress = [&camera](XEvent& event) mutable {
        math::vec3& pos = camera.getPos();
        math::vec3* axis;
        bool sub = false;
        int yaw = 0;
        int pitch = 0;
        switch (event.xkey.keycode) {
            case 25: // W
                axis = &camera.cameraMatrix.c;
                break;
            case 24: // Q
                axis = &camera.cameraMatrix.b;
                break;
            case 26: // E
                axis = &camera.cameraMatrix.b;
                sub = true;
                break;
            case 38: // A
                axis = &camera.cameraMatrix.a;
                break;
            case 39: // S
                axis = &camera.cameraMatrix.c;
                sub = true;
                break;
            case 40: // D
                axis = &camera.cameraMatrix.a;
                sub = true;
                break;
            case 111: // Up
                pitch = -1;
                break;
            case 113: // Left
                yaw = 1;
                break;
            case 114: // Right
                yaw = -1;
                break;
            case 116: // Down
                pitch = 1;
                break;
            default:
                std::cout << event.xkey.keycode << std::endl;
        }
        float m = 0.1;
        sub ? pos.x -= m*axis->x : pos.x += m*axis->x;
        sub ? pos.y -= m*axis->y : pos.y += m*axis->y;
        sub ? pos.z -= m*axis->z : pos.z += m*axis->z;
        camera.rotateLookAt(yaw, pitch, 0);
    };

    auto onKeyRelease = [&camera](XEvent& event) mutable {
        std::cout << "Key released: " << XLookupKeysym(&event.xkey, 0) << std::endl;
    };

    auto onButtonPress = [&camera](XEvent& event) mutable {
        std::cout << "Mouse Button " << event.xbutton.button << " Pressed at (" << event.xbutton.x  << ", " << event.xbutton.y << ")" << std::endl;
    };

    auto onButtonRelease = [&camera](XEvent& event) mutable {
        std::cout << "Mouse Button " << event.xbutton.button << " Released at (" << event.xbutton.x << ", " << event.xbutton.y << ")" << std::endl;
    };

    auto onMouseMotion = [&camera](XEvent& event) mutable {
        std::cout << "Mouse Moved to (" << event.xmotion.x << ", " << event.xmotion.y << ")" << std::endl;
    };

    DisplayX11 display(480, 360);
    display.addListener(EXPOSE, onExpose);
    display.addListener(KEY_PRESS, onKeyPress);
    display.addListener(KEY_RELEASE, onKeyRelease);
    display.addListener(BUTTON_PRESS, onButtonPress);
    display.addListener(BUTTON_RELEASE, onButtonRelease);
    display.addListener(MOUSE_MOTION, onMouseMotion);

    Rasterizer renderer(display, scene);

    bool running = true;
    while (running) {
        std::cout << "tick" << std::endl;
        display.clear();
        display.clearZBuffer();

        camera.updateViewTransformation();
        renderer.updateProjectionMatrix();

        renderer.render();

        display.update();
    }
}