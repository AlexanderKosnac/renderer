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
    modelling::Camera cam(90.0f, .75f, 0.1f, 1000.0f, math::vec3(0.0f, 0.0f, 0.0f), math::vec3(0.0f, 1.0f, 0.0f), math::vec3(0.0f, 0.0f, 1.0f));

    // Origin Cube
    // - Vertices
    math::vec3 v0 = math::vec3(0.0f, 0.0f, 0.0f);
    math::vec3 v1 = math::vec3(0.0f, 1.0f, 0.0f);
    math::vec3 v2 = math::vec3(1.0f, 0.0f, 0.0f);
    math::vec3 v3 = math::vec3(1.0f, 1.0f, 0.0f);
    math::vec3 v4 = math::vec3(0.0f, 0.0f, 1.0f);
    math::vec3 v5 = math::vec3(0.0f, 1.0f, 1.0f);
    math::vec3 v6 = math::vec3(1.0f, 0.0f, 1.0f);
    math::vec3 v7 = math::vec3(1.0f, 1.0f, 1.0f);
    // - Colors
    math::vec3 c0 = math::vec3(255.0f, 0.0f, 0.0f);
    math::vec3 c1 = math::vec3(255.0f, 0.0f, 0.0f);
    math::vec3 c2 = math::vec3(255.0f, 0.0f, 0.0f);
    math::vec3 c3 = math::vec3(255.0f, 0.0f, 0.0f);
    math::vec3 c4 = math::vec3(255.0f, 0.0f, 0.0f);
    math::vec3 c5 = math::vec3(255.0f, 0.0f, 0.0f);
    math::vec3 c6 = math::vec3(255.0f, 0.0f, 0.0f);
    math::vec3 c7 = math::vec3(255.0f, 0.0f, 0.0f);
    // - Faces
    modelling::Triangle f0;
    f0.setVertex(0, v0, c0);
    f0.setVertex(1, v2, c2);
    f0.setVertex(2, v3, c3);

    modelling::Triangle f1;
    f1.setVertex(0, v0, c0);
    f1.setVertex(1, v3, c3);
    f1.setVertex(2, v1, c1);

    modelling::Triangle f2;
    f2.setVertex(0, v1, c1);
    f2.setVertex(1, v3, c3);
    f2.setVertex(2, v7, c7);

    modelling::Triangle f3;
    f3.setVertex(0, v1, c1);
    f3.setVertex(1, v7, c7);
    f3.setVertex(2, v5, c5);

    modelling::Triangle f4;
    f4.setVertex(0, v0, c0);
    f4.setVertex(1, v4, c4);
    f4.setVertex(2, v6, c6);

    modelling::Triangle f5;
    f5.setVertex(0, v0, c0);
    f5.setVertex(1, v6, c6);
    f5.setVertex(2, v2, c2);

    modelling::Triangle f6;
    f6.setVertex(0, v2, c2);
    f6.setVertex(1, v6, c6);
    f6.setVertex(2, v3, c3);

    modelling::Triangle f7;
    f7.setVertex(0, v3, c3);
    f7.setVertex(1, v6, c6);
    f7.setVertex(2, v7, c7);

    modelling::Triangle f8;
    f8.setVertex(0, v0, c0);
    f8.setVertex(1, v1, c1);
    f8.setVertex(2, v4, c4);

    modelling::Triangle f9;
    f9.setVertex(0, v1, c1);
    f9.setVertex(1, v5, c5);
    f9.setVertex(2, v4, c4);

    modelling::Triangle f10;
    f10.setVertex(0, v5, c5);
    f10.setVertex(1, v6, c6);
    f10.setVertex(2, v4, c4);

    modelling::Triangle f11;
    f11.setVertex(0, v5, c5);
    f11.setVertex(1, v7, c7);
    f11.setVertex(2, v6, c6);

    modelling::Mesh mesh;
    mesh.addTriangle(f0);
    mesh.addTriangle(f1);
    mesh.addTriangle(f2);
    mesh.addTriangle(f3);
    mesh.addTriangle(f4);
    mesh.addTriangle(f5);
    mesh.addTriangle(f6);
    mesh.addTriangle(f7);
    mesh.addTriangle(f8);
    mesh.addTriangle(f9);
    mesh.addTriangle(f10);
    mesh.addTriangle(f11);

    Scene scene(cam);
    scene.addMesh(mesh);

    modelling::Camera& camera = scene.getCamera();

    auto onExpose = [&camera](XEvent& event) mutable {
        std::cout << "Expose" << std::endl;
    };

    auto onKeyPress = [&camera](XEvent& event) mutable {
        math::vec3& pos = camera.getPos();
        math::vec3 axis;
        bool sub = false;
        switch (event.xkey.keycode) {
            case 25: // W
                axis = camera.normedW();
                break;
            case 38: // A
                axis = camera.normedU();
                sub = true;
                break;
            case 39: // S
                axis = camera.normedW();
                sub = true;
                break;
            case 40: // D
                axis = camera.normedU();
                break;
        }
        sub ? pos.x -= axis.x : pos.x += axis.x;
        sub ? pos.y -= axis.y : pos.y += axis.y;
        sub ? pos.z -= axis.z : pos.z += axis.z;
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
        renderer.render();
        display.update();
    }
}
