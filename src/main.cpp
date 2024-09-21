#include <X11/Xlib.h>

#include <iostream>
#include <chrono>
#include <cmath>
#include <vector>
#include <fmt/core.h>

#include "math.h"
#include "modelling/triangle.h"
#include "modelling/mesh.h"
#include "modelling/camera.h"
#include "modelling/scene.h"
#include "modelling/transformations.h"
#include "rasterizer/rasterizer.h"
#include "display/x11display.h"
#include "display/callbacktypes.h"

int main(int argc, char* argv[]) {
    modelling::Camera cam(90.0f, 4.0/3.0, 0.1f, 1000.0f, math::vec3(0.0f, 0.0f, 0.0f), math::vec3(0.0f, 1.0f, 0.0f), math::vec3(0.0f, 0.0f, -1.0f));

    math::vec3 light(0.0f, 1.0f, 0.0f);

    Scene scene(cam, light);

    if (argc > 1) {
        std::string select = argv[1];
        if (select == "cube") {
        } else if (select == "axis") {
            scene.addObject(modelling::Mesh("objs/axis.obj"), {
                transformation::translate(0.0f, 0.0f, 0.0f),
        if (select == "triangle") {
            scene.addObject(modelling::Mesh("objs/triangle.obj"), {
                transformation::translate(0.0f, 0.0f, -3.0f),
            });
        } else if (select == "axis") {
            scene.addObject(modelling::Mesh("objs/axis.obj"), {});
            scene.addObject(modelling::Mesh("objs/cube.obj"), {
                transformation::rotationY(30.0f).toMat4x4(),
                transformation::rotationX(30.0f).toMat4x4(),
                transformation::translate(0.0f, 0.0f, 10.0f),
            });
        } else if (select == "d20") {
            scene.addObject(modelling::Mesh("objs/d20.obj"), {
                transformation::scale(2.0f).toMat4x4(),
                transformation::rotationX(10.0f).toMat4x4(),
                transformation::translate(0.0f, 0.0f, 20.0f),
            });
        } else if (select == "kokiri") {
            scene.addObject(modelling::Mesh("objs/kokiri.obj"), {
                transformation::scale(0.1f).toMat4x4(),
                transformation::rotationY(165.0f).toMat4x4(),
                transformation::translate(0.0f, -50.0f, 20.0f),
            });
        } else if (select == "psyduck") {
            scene.addObject(modelling::Mesh("objs/psyduck.obj"), {
                transformation::scale(0.01f).toMat4x4(),
                transformation::rotationY(180.0f).toMat4x4(),
                transformation::translate(0.0f, -2.0f, 20.0f),
            });
        } else if (select == "teapot") {
            scene.addObject(modelling::Mesh("objs/teapot.obj"), {
                transformation::translate(0.0f, -2.0f, 20.0f),
            });
        } else {
            fprintf(stderr, "Unknown scene '%s'. Check available scenes.\n", select.c_str());
            return 1;
        }
    } else {
        // Default scene is the Utah Teapot
        scene.addObject(modelling::Mesh("objs/teapot.obj"), {
            transformation::translate(0.0f, -2.0f, 20.0f),
        });
    }

    modelling::Camera& camera = scene.getCamera();

    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunused-parameter"
    auto onExpose = [&camera](XEvent& event) mutable {};
    #pragma GCC diagnostic pop

    auto onKeyPress = [&camera](XEvent& event) mutable {
        math::vec3& pos = camera.getPos();
        float m = 1.5;
        float degree = 1.0f;
        switch (event.xkey.keycode) {
            case 25: // W
                pos.x += m*camera.cameraMatrix.c.x;
                pos.y += m*camera.cameraMatrix.c.y;
                pos.z += m*camera.cameraMatrix.c.z;
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
                pos.x += m*camera.cameraMatrix.a.x;
                pos.y += m*camera.cameraMatrix.a.y;
                pos.z += m*camera.cameraMatrix.a.z;
                break;
            case 39: // S
                pos.x -= m*camera.cameraMatrix.c.x;
                pos.y -= m*camera.cameraMatrix.c.y;
                pos.z -= m*camera.cameraMatrix.c.z;
                break;
            case 40: // D
                pos.x -= m*camera.cameraMatrix.a.x;
                pos.y -= m*camera.cameraMatrix.a.y;
                pos.z -= m*camera.cameraMatrix.a.z;
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

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    int ms;

    bool running = true;
    while (running) {
        display.clear();
        display.clearZBuffer();

        camera.updateViewTransformation();
        renderer.updateProjectionMatrix();

        renderer.render();

        end = std::chrono::steady_clock::now();
        ms = std::chrono::duration_cast<std::chrono::milliseconds> (end - begin).count();
        display.setWindowTitle(fmt::format("Renderer [Tick in {} ms, approx. {:.2f} fps]", ms, 1000.0f/((ms == 0) ? 1 : ms)));
        display.update();
        begin = std::chrono::steady_clock::now();
    }
}