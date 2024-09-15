#include <X11/Xlib.h>

#include <iostream>
#include <vector>

#include "math.h"
#include "modelling/triangle.h"
#include "modelling/mesh.h"
#include "modelling/camera.h"
#include "modelling/scene.h"
#include "modelling/transformations.h"
#include "rasterizer/rasterizer.h"
#include "display/x11display.h"
#include "display/callbacktypes.h"

int main() {
    modelling::Camera cam(90.0f, 4.0/3.0, 0.1f, 1000.0f, math::vec3(0.0f, 0.0f, 0.0f), math::vec3(0.0f, 1.0f, 0.0f), math::vec3(0.0f, 0.0f, -1.0f));

    //modelling::Mesh mesh("objs/axis.obj");
    modelling::Mesh axisMesh("objs/axis.obj");
    modelling::Mesh mesh("objs/cube.obj");

    math::vec3 light(0.0f, -1.0f, 0.0f);

    Scene scene(cam, light);

    scene.addObject(axisMesh, {
        transformation::scaleXYZ(0.1f, 0.1f, 0.1f).toMat4x4(),
        transformation::translate(0.0f, 0.0f, -100.0f),
    });

    scene.addObject(mesh, {
        transformation::translate(0.0f, 0.0f, -100.0f),
    });

    modelling::Camera& camera = scene.getCamera();

    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunused-parameter"
    auto onExpose = [&camera](XEvent& event) mutable {};
    #pragma GCC diagnostic pop

    auto onKeyPress = [&camera](XEvent& event) mutable {
        math::vec3& pos = camera.getPos();
        float m = 0.5;
        float degree = 1.0f;
        switch (event.xkey.keycode) {
            case 25: // W
                pos.x -= m*camera.cameraMatrix.c.x;
                pos.y -= m*camera.cameraMatrix.c.y;
                pos.z -= m*camera.cameraMatrix.c.z;
                break;
            case 24: // Q
                pos.x += m*camera.cameraMatrix.b.x;
                pos.y += m*camera.cameraMatrix.b.y;
                pos.z += m*camera.cameraMatrix.b.z;
                break;
            case 26: // E
                pos.x -= m*camera.cameraMatrix.b.x;
                pos.y -= m*camera.cameraMatrix.b.y;
                pos.z -= m*camera.cameraMatrix.b.z;
                break;
            case 38: // A
                pos.x -= m*camera.cameraMatrix.a.x;
                pos.y -= m*camera.cameraMatrix.a.y;
                pos.z -= m*camera.cameraMatrix.a.z;
                break;
            case 39: // S
                pos.x += m*camera.cameraMatrix.c.x;
                pos.y += m*camera.cameraMatrix.c.y;
                pos.z += m*camera.cameraMatrix.c.z;
                break;
            case 40: // D
                pos.x += m*camera.cameraMatrix.a.x;
                pos.y += m*camera.cameraMatrix.a.y;
                pos.z += m*camera.cameraMatrix.a.z;
                break;
            case 111: // Up
                camera.pitchLookAt(-degree);
                break;
            case 113: // Left
                camera.yawLookAt(degree);
                break;
            case 114: // Right
                camera.yawLookAt(-degree);
                break;
            case 116: // Down
                camera.pitchLookAt(degree);
                break;
            default:
                std::cout << event.xkey.keycode << std::endl;
        }
    };

    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunused-parameter"
    auto onKeyRelease = [&camera](XEvent& event) mutable {};
    #pragma GCC diagnostic pop

    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunused-parameter"
    auto onButtonPress = [&camera](XEvent& event) mutable {};
    #pragma GCC diagnostic pop

    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunused-parameter"
    auto onButtonRelease = [&camera](XEvent& event) mutable {};
    #pragma GCC diagnostic pop

    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunused-parameter"
    auto onMouseMotion = [&camera](XEvent& event) mutable {};
    #pragma GCC diagnostic pop

    DisplayX11 display(960, 720);
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