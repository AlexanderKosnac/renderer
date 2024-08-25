#include "rasterizer/rasterizer.h"

#include "display/x11display.h"
#include "modelling/scene.h"

Rasterizer::Rasterizer(DisplayX11& pDisplay, Scene& pScene) : display(pDisplay), scene(pScene) {}

void Rasterizer::render() {
    display.setImage();
}