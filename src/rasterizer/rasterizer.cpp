#include "rasterizer/rasterizer.h"

#include "math.h"
#include "display/x11display.h"
#include "modelling/scene.h"

Rasterizer::Rasterizer(DisplayX11& pDisplay, Scene& pScene) : display(pDisplay), scene(pScene) {
    updateProjectionMatrix();
}

void Rasterizer::render() {
    float width2 = 0.5f * display.getWidth();
    float height2 = 0.5f * display.getHeight();
    for (auto mesh : scene.getMeshes()) {
        for (auto triangle : mesh.getTriangles()) {
            math::vec3 v0 = math::multMat4x4OnVec4(projectionMatrix, triangle.getVertexPos(0).toVec4(1)).dehomogenize();
            math::vec3 v1 = math::multMat4x4OnVec4(projectionMatrix, triangle.getVertexPos(1).toVec4(1)).dehomogenize();
            math::vec3 v2 = math::multMat4x4OnVec4(projectionMatrix, triangle.getVertexPos(2).toVec4(1)).dehomogenize();

            v0.x += 1.0f;
            v0.y += 1.0f;
            v1.x += 1.0f;
            v1.y += 1.0f;
            v2.x += 1.0f;
            v2.y += 1.0f;

            v0.x *= width2;
            v0.y *= height2;
            v1.x *= width2;
            v1.y *= height2;
            v2.x *= width2;
            v2.y *= height2;

            modelling::Triangle projected;
            projected.setVertex(0, v0, triangle.getVertexColor(0));
            projected.setVertex(1, v1, triangle.getVertexColor(1));
            projected.setVertex(2, v2, triangle.getVertexColor(2));

            drawTriangle(projected);
        }
    }
}

void Rasterizer::updateProjectionMatrix() {
    scene.getCamera().setProjectionMatrix(projectionMatrix);
}

void Rasterizer::drawLine(float x1, float y1, math::vec3& color1, float x2, float y2, math::vec3& color2) {
    int x, y, xe, ye;
    int dx = x2 - x1;
    int dy = y2 - y1;
    int dx1 = abs(dx);
    int dy1 = abs(dy);
    int px = 2 * dy1 - dx1;
    int py = 2 * dx1 - dy1;

    math::vec3 colorA, colorB;

    if (dy1 <= dx1) {
        if (dx < 0) {
            x = x2;
            y = y2;
            xe = x1;
            colorA = color1;
            colorB = color2;
        } else {
            x = x1;
            y = y1;
            xe = x2;
            colorA = color2;
            colorB = color1;
        }

        display.setPixel(x, y, colorB);

        for (int i=0; x<xe; i++) {
            x += 1;
            if (px < 0) {
                px = px + 2 * dy1;
            } else {
                ((dx<0 && dy<0) || (dx>0 && dy>0)) ? y++ : y--;
                px = px + 2 * (dy1 - dx1);
            }
            math::vec3 color = math::linInterpolVec3((float)i/(float)dx1, colorA, colorB);
            display.setPixel(x, y, color);
        }
    } else {
        if (dy < 0) {
            x = x2;
            y = y2;
            ye = y1;
            colorA = color1;
            colorB = color2;
        } else {
            x = x1;
            y = y1;
            ye = y2;
            colorA = color2;
            colorB = color1;
        }

        display.setPixel(x, y, colorB);

        for (int i=0; y<ye; i++) {
            y += 1;
            if (py > 0) {
                ((dx<0 && dy<0) || (dx>0 && dy>0)) ? x++ : x--;
                py = py + 2 * (dx1 - dy1);
            } else {
                py = py + 2 * dx1;
            }
            math::vec3 color = math::linInterpolVec3((float)i/(float)dy1, colorA, colorB);
            display.setPixel(x, y, color);
        }
    }
}

void Rasterizer::drawTriangle(modelling::Triangle& t) {
    math::vec3& v0 = t.getVertexPos(0);
    math::vec3& v1 = t.getVertexPos(1);
    math::vec3& v2 = t.getVertexPos(2);

    drawLine(v0.x, v0.y, t.color[0], v1.x, v1.y, t.color[1]);
    drawLine(v1.x, v1.y, t.color[1], v2.x, v2.y, t.color[2]);
    drawLine(v2.x, v2.y, t.color[2], v0.x, v0.y, t.color[0]);
}