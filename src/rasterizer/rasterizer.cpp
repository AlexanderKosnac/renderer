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

void Rasterizer::drawLine(float x1, float y1, float x2, float y2) {
    math::vec3 color = math::vec3(0.0f, 255.0f, 0.0f);

    int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
    dx = x2 - x1;
    dy = y2 - y1;
    dx1 = abs(dx);
    dy1 = abs(dy);
    px = 2 * dy1 - dx1;
    py = 2 * dx1 - dy1;
    if (dy1 <= dx1) {
        if (dx >= 0) {
            x = x1;
            y = y1;
            xe = x2;
        } else {
            x = x2;
            y = y2;
            xe = x1;
        }

        display.setPixel(x, y, color);

        for (i=0; x<xe; i++) {
            x = x + 1;
            if (px<0) {
                px = px + 2 * dy1;
            } else {
                ((dx<0 && dy<0) || (dx>0 && dy>0)) ? y++ : y--;
                px = px + 2 * (dy1 - dx1);
            }
            display.setPixel(x, y, color);
        }
    } else {
        if (dy >= 0) {
            x = x1;
            y = y1;
            ye = y2;
        } else {
            x = x2;
            y = y2;
            ye = y1;
        }

        display.setPixel(x, y, color);

        for (i=0; y<ye; i++) {
            y = y + 1;
            if (py <= 0) {
                py = py + 2 * dx1;
            } else {
                ((dx<0 && dy<0) || (dx>0 && dy>0)) ? x++ : x--;
                py = py + 2 * (dx1 - dy1);
            }
            display.setPixel(x, y, color);
        }
    }
}

void Rasterizer::drawTriangle(modelling::Triangle& t) {
    math::vec3& v0 = t.getVertexPos(0);
    math::vec3& v1 = t.getVertexPos(1);
    math::vec3& v2 = t.getVertexPos(2);
    drawLine(v0.x, v0.y, v1.x, v1.y);
    drawLine(v1.x, v1.y, v2.x, v2.y);
    drawLine(v2.x, v2.y, v0.x, v0.y);
}