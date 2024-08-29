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

    void drawLine(float x1, float y1, math::vec3& color1, float x2, float y2, math::vec3& color2);
    void drawTriangle(modelling::Triangle& t);
};

#endif