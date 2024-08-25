#ifndef RASTERIZER
#define RASTERIZER

#include "display/x11display.h"
#include "modelling/scene.h"

class Rasterizer {
private:
    DisplayX11& display;
    Scene& scene;

public:
    Rasterizer(DisplayX11& pDisplay, Scene& pScene);

    void render();
};

#endif