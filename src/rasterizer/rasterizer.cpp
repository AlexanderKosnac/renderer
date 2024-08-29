#include "rasterizer/rasterizer.h"

#include "math.h"
#include "display/x11display.h"
#include "modelling/scene.h"

Rasterizer::Rasterizer(DisplayX11& pDisplay, Scene& pScene) : display(pDisplay), scene(pScene) {
    updateProjectionMatrix();
}

void Rasterizer::render() {
    display.setImage();
void Rasterizer::updateProjectionMatrix() {
    scene.getCamera().setProjectionMatrix(projectionMatrix);
}

}