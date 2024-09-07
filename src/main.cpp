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
    math::vec3 c0 = math::vec3(255.0f,   0.0f,   0.0f);
    math::vec3 c1 = math::vec3(  0.0f, 255.0f,   0.0f);
    math::vec3 c2 = math::vec3(  0.0f,   0.0f, 255.0f);
    math::vec3 c3 = math::vec3(255.0f, 255.0f,   0.0f);
    math::vec3 c4 = math::vec3(255.0f,   0.0f, 255.0f);
    math::vec3 c5 = math::vec3(  0.0f, 255.0f, 255.0f);
    math::vec3 c6 = math::vec3(255.0f, 128.0f,   0.0f);
    math::vec3 c7 = math::vec3(255.0f,   0.0f, 128.0f);
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

    math::vec3 light(0.0f, -1.0f, 0.0f);

    Scene scene(cam, light);
    scene.addMesh(mesh);

    for (auto i : { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }) {
        modelling::Triangle t0;
        t0.setVertex(0, math::vec3(0.0f, -1.0f, 0.0f+i), c0);
        t0.setVertex(1, math::vec3(0.0f, -1.0f, 0.5f+i), c0);
        t0.setVertex(2, math::vec3(1.0f, -1.0f, 0.5f+i), c0);

        modelling::Triangle t1;
        t1.setVertex(0, math::vec3(0.0f+i, -1.0f, 0.0f), c1);
        t1.setVertex(1, math::vec3(0.0f+i, -1.0f, 0.5f), c1);
        t1.setVertex(2, math::vec3(1.0f+i, -1.0f, 0.5f), c1);

        modelling::Triangle t2;
        t2.setVertex(0, math::vec3(0.0f, -1.0f, 0.0f-i), c2);
        t2.setVertex(1, math::vec3(0.0f, -1.0f, 0.5f-i), c2);
        t2.setVertex(2, math::vec3(1.0f, -1.0f, 0.5f-i), c2);

        modelling::Triangle t3;
        t3.setVertex(0, math::vec3(0.0f-i, -1.0f, 0.0f), c3);
        t3.setVertex(1, math::vec3(0.0f-i, -1.0f, 0.5f), c3);
        t3.setVertex(2, math::vec3(1.0f-i, -1.0f, 0.5f), c3);

        modelling::Mesh m;
        m.addTriangle(t0);
        m.addTriangle(t1);
        m.addTriangle(t2);
        m.addTriangle(t3);

        scene.addMesh(m);
    }

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
