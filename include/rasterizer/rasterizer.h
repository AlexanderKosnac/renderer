#ifndef RASTERIZER
#define RASTERIZER

#include "math.h"
#include "display/x11display.h"
#include "modelling/scene.h"

class Rasterizer {
private:
    DisplayX11& display;
    Scene& scene;

    math::mat4x4 projectionMatrix;

public:
    Rasterizer(DisplayX11& pDisplay, Scene& pScene);

    void render();

    void updateProjectionMatrix();
};

#endif