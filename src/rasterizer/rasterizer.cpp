#include "rasterizer/rasterizer.h"

#include <algorithm>

#include "math.h"
#include "display/x11display.h"
#include "modelling/camera.h"
#include "modelling/scene.h"

Rasterizer::Rasterizer(DisplayX11& pDisplay, Scene& pScene) : display(pDisplay), scene(pScene) {
    updateProjectionMatrix();
}

void Rasterizer::render() {
    math::mat4x4 viewProjectionMatrix = math::multMat4x4OnMat4x4(projectionMatrix, scene.getCamera().uvw);

    float width2 = 0.5f * display.getWidth();
    float height2 = 0.5f * display.getHeight();
    for (auto mesh : scene.getMeshes()) {
        for (auto triangle : mesh.getTriangles()) {
            modelling::Triangle projected;
            for (auto i : { 0, 1, 2 }) {
                math::vec3 v;
                v = triangle.getVertexPos(i);
                v = math::multMat4x4OnVec4(viewProjectionMatrix, v.toVec4(1)).dehomogenize();

                v.x = (v.x + 1.0f) * width2;
                v.y = (v.y + 1.0f) * height2;
                projected.setVertex(i, v, triangle.getVertexColor(i));
            }
            fillTriangle(projected);
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

        display.setPixel(x, y, 0, colorB);

        for (int i=0; x<xe; i++) {
            x += 1;
            if (px < 0) {
                px = px + 2 * dy1;
            } else {
                ((dx<0 && dy<0) || (dx>0 && dy>0)) ? y++ : y--;
                px = px + 2 * (dy1 - dx1);
            }
            math::vec3 color = math::linInterpolVec3((float)i/(float)dx1, colorA, colorB);
            display.setPixel(x, y, 0, color);
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

        display.setPixel(x, y, 0, colorB);

        for (int i=0; y<ye; i++) {
            y += 1;
            if (py > 0) {
                ((dx<0 && dy<0) || (dx>0 && dy>0)) ? x++ : x--;
                py = py + 2 * (dx1 - dy1);
            } else {
                py = py + 2 * dx1;
            }
            math::vec3 color = math::linInterpolVec3((float)i/(float)dy1, colorA, colorB);
            display.setPixel(x, y, 0, color);
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

void Rasterizer::fillTriangle(modelling::Triangle& t) {
    const math::vec3 v0 = t.getVertexPos(0);
    const math::vec3 v1 = t.getVertexPos(1);
    const math::vec3 v2 = t.getVertexPos(2);
    const math::vec3 c0 = t.getVertexColor(0);
    const math::vec3 c1 = t.getVertexColor(1);
    const math::vec3 c2 = t.getVertexColor(2);

    // Flattened sort
    float minx = std::min({ v0.x, v1.x, v2.x });
    float miny = std::min({ v0.y, v1.y, v2.y });
    float maxx = std::max({ v0.x, v1.x, v2.x });
    float maxy = std::max({ v0.y, v1.y, v2.y });

    for (int x=minx; x<=maxx; x++) {
        for (int y=miny; y<=maxy; y++) {
            math::vec3 p = math::vec3(x, y, 0.0f);
            float l0, l1, l2;
            math::barycentric(v0, v1, v2, p, l0, l1, l2);
            if (l0 > 0 && l1 > 0 && l2 > 0) {
                math::vec3 color = math::vec3(
                    c0.x * l0 + c1.x * l1 + c2.x * l2,
                    c0.y * l0 + c1.y * l1 + c2.y * l2,
                    c0.z * l0 + c1.z * l1 + c2.z * l2
                );
                float z = v0.z*l0 + v1.z*l1 + v2.z*l2;
                display.setPixel(x, y, z, color);
            }
        }
    }

}