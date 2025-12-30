#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <fmt/core.h>

#include <chrono>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

#include "display/callbacktypes.h"
#include "display/framebuffer.h"
#include "display/x11display.h"
#include "math.h"
#include "modelling/camera.h"
#include "modelling/mesh.h"
#include "modelling/scene.h"
#include "modelling/transformations.h"
#include "modelling/triangle.h"
#include "rasterizer/rasterizer.h"

struct AppConfig {
    int displayWidth = 640;
    int displayHeight = 480;

    int fbWidth = 64 * 4;
    int fbHeight = 48 * 4;

    std::string scene = "teapot";
};

static void parseDimension(const std::string &s, int &w, int &h) {
    auto pos = s.find('x');
    if (pos == std::string::npos) {
        throw std::runtime_error("Invalid dimension format (expected WxH)");
    }

    w = std::stoi(s.substr(0, pos));
    h = std::stoi(s.substr(pos + 1));

    if (w <= 0 || h <= 0) {
        throw std::runtime_error("Dimensions must be positive");
    }
}

AppConfig parseArgs(int argc, char *argv[]) {
    AppConfig cfg;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "--scene" && i + 1 < argc) {
            cfg.scene = argv[++i];
        } else if (arg == "--display" && i + 1 < argc) {
            parseDimension(argv[++i], cfg.displayWidth, cfg.displayHeight);
        } else if (arg == "--framebuffer" && i + 1 < argc) {
            parseDimension(argv[++i], cfg.fbWidth, cfg.fbHeight);
        } else {
            throw std::runtime_error("Unknown or incomplete argument: " + arg);
        }
    }

    return cfg;
}

int main(int argc, char *argv[]) {
    AppConfig cfg = parseArgs(argc, argv);

    DisplayX11 display(cfg.displayWidth, cfg.displayHeight);
    Framebuffer framebuffer(cfg.fbWidth, cfg.fbHeight);

    modelling::Camera cam(60.0f, 4.0 / 3.0, 0.1f, 1000.0f, math::vec3(0.0f, 0.0f, 0.0f), math::vec3(0.0f, 1.0f, 0.0f), math::vec3(0.0f, 0.0f, 1.0f));
    math::vec3 light(0.0f, -1.0f, 0.0f);

    Scene scene(cam, light);

    modelling::Camera &camera = scene.getCamera();
    Rasterizer renderer(framebuffer, scene);

    const std::string &select = cfg.scene;
    if (select == "triangle") {
        scene.addObject(modelling::Mesh("objs/triangle.obj"), {
                                                                  transformation::translate(0.0f, 0.0f, -3.0f),
                                                              });
    } else if (select == "axis") {
        scene.addObject(modelling::Mesh("objs/axis.obj"), {});
    } else if (select == "cube") {
        scene.addObject(modelling::Mesh("objs/cube.obj"), {
                                                              transformation::rotationY(30.0f).toMat4x4(),
                                                              transformation::rotationX(30.0f).toMat4x4(),
                                                              transformation::translate(0.0f, 0.0f, -3.0f),
                                                          });
    } else if (select == "d20") {
        scene.addObject(modelling::Mesh("objs/d20.obj"), {
                                                             transformation::scale(2.0f).toMat4x4(),
                                                             transformation::rotationX(10.0f).toMat4x4(),
                                                             transformation::translate(0.0f, 0.0f, -20.0f),
                                                         });
    } else if (select == "kokiri") {
        scene.addObject(modelling::Mesh("objs/kokiri.obj"), {
                                                                transformation::scale(0.1f).toMat4x4(),
                                                                transformation::rotationY(165.0f).toMat4x4(),
                                                                transformation::translate(0.0f, -50.0f, -20.0f),
                                                            });
    } else if (select == "psyduck") {
        scene.addObject(modelling::Mesh("objs/psyduck.obj"), {
                                                                 transformation::scale(0.01f).toMat4x4(),
                                                                 transformation::translate(0.0f, -2.0f, -10.0f),
                                                             });
    } else if (select == "teapot") {
        scene.addObject(modelling::Mesh("objs/teapot.obj"), {
                                                                transformation::translate(0.0f, -2.0f, -5.0f),
                                                            });
    } else {
        fprintf(stderr, "Unknown scene '%s'. Check available scenes.\n", select.c_str());
        return 1;
    }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
    auto onExpose = [&camera](XEvent &event) mutable {};
#pragma GCC diagnostic pop

    auto onKeyPress = [&camera](XEvent &event) mutable {
        math::vec3 &pos = camera.getPos();
        float m = 1.0f;
        float degree = 1.0f;
        switch (event.xkey.keycode) {
        case 25: // W
            pos.x += m * camera.cameraMatrix.c.x;
            pos.y += m * camera.cameraMatrix.c.y;
            pos.z += m * camera.cameraMatrix.c.z;
            break;
        case 24: // Q
            pos.x += m * camera.cameraMatrix.b.x;
            pos.y += m * camera.cameraMatrix.b.y;
            pos.z += m * camera.cameraMatrix.b.z;
            break;
        case 26: // E
            pos.x -= m * camera.cameraMatrix.b.x;
            pos.y -= m * camera.cameraMatrix.b.y;
            pos.z -= m * camera.cameraMatrix.b.z;
            break;
        case 38: // A
            pos.x += m * camera.cameraMatrix.a.x;
            pos.y += m * camera.cameraMatrix.a.y;
            pos.z += m * camera.cameraMatrix.a.z;
            break;
        case 39: // S
            pos.x -= m * camera.cameraMatrix.c.x;
            pos.y -= m * camera.cameraMatrix.c.y;
            pos.z -= m * camera.cameraMatrix.c.z;
            break;
        case 40: // D
            pos.x -= m * camera.cameraMatrix.a.x;
            pos.y -= m * camera.cameraMatrix.a.y;
            pos.z -= m * camera.cameraMatrix.a.z;
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
    auto onKeyRelease = [&camera](XEvent &event) mutable {};
#pragma GCC diagnostic pop

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
    auto onButtonPress = [&camera](XEvent &event) mutable {};
#pragma GCC diagnostic pop

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
    auto onButtonRelease = [&camera](XEvent &event) mutable {};
#pragma GCC diagnostic pop

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
    auto onMouseMotion = [&camera](XEvent &event) mutable {};
#pragma GCC diagnostic pop

    auto onWindowResize = [&camera, &display](XEvent &event) mutable {
        int w = event.xconfigure.width;
        int h = event.xconfigure.height;
        display.setDimensions(w, h);
    };

    display.addListener(EXPOSE, onExpose);
    display.addListener(KEY_PRESS, onKeyPress);
    display.addListener(KEY_RELEASE, onKeyRelease);
    display.addListener(BUTTON_PRESS, onButtonPress);
    display.addListener(BUTTON_RELEASE, onButtonRelease);
    display.addListener(MOUSE_MOTION, onMouseMotion);
    display.addListener(WINDOW_RESIZE, onWindowResize);

    using clock = std::chrono::steady_clock;

    constexpr double TARGET_FPS = 30.0;
    constexpr auto FRAME_TIME = std::chrono::duration<double, std::milli>(1000.0 / TARGET_FPS);

    bool running = true;

    while (running) {
        auto frameStart = clock::now();

        framebuffer.clear();
        framebuffer.clearZ();

        camera.updateViewTransformation();
        renderer.updateProjectionMatrix();
        renderer.render();

        display.pollEvents();
        display.present(framebuffer);

        auto frameEnd = clock::now();
        auto frameDuration = frameEnd - frameStart;

        double ms = std::chrono::duration<double, std::milli>(frameDuration).count();
        display.setWindowTitle(fmt::format("Renderer [{:.2f} ms per frame | {:.1f} fps (capped to {:.1f})]", ms, 1000.0 / std::max(ms, 0.001), TARGET_FPS));

        if (frameDuration < FRAME_TIME) {
            std::this_thread::sleep_for(FRAME_TIME - frameDuration);
        }
    }
}