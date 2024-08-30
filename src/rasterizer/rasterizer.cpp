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
            modelling::Triangle projected;
            for (auto i : { 0, 1, 2 }) {
                math::vec3 v = math::multMat4x4OnVec4(projectionMatrix, triangle.getVertexPos(i).toVec4(1)).dehomogenize();
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

template <typename T>
void swap(T& a, T& b) {
    T temp = a;
    a = b;
    b = temp;
}

void Rasterizer::fillTriangle(modelling::Triangle& t) {
    const math::vec3* p0 = &t.getVertexPos(0);
    const math::vec3* p1 = &t.getVertexPos(1);
    const math::vec3* p2 = &t.getVertexPos(2);
    const math::vec3* c0 = &t.getVertexColor(0);
    const math::vec3* c1 = &t.getVertexColor(1);
    const math::vec3* c2 = &t.getVertexColor(2);

    // Flattened sort
    if (p1->y < p0->y) { swap(p0, p1); swap(c0, c1); }
    if (p2->y < p0->y) { swap(p0, p2); swap(c0, c2); }
    if (p2->y < p1->y) { swap(p1, p2); swap(c1, c2); }

    std::cout << "p0 (" << p0->x << "," << p0->y << "," << p0->z << ") [" << c0->x << "," << c0->y << "," << c0->z << "]" << std::endl;
    std::cout << "p1 (" << p1->x << "," << p1->y << "," << p1->z << ") [" << c1->x << "," << c1->y << "," << c1->z << "]" << std::endl;
    std::cout << "p2 (" << p2->x << "," << p2->y << "," << p2->z << ") [" << c2->x << "," << c2->y << "," << c2->z << "]" << std::endl;

    math::vec3 top = *p0, mid = *p1, bot = *p2;
    math::vec3 topColor = *c0, midColor = *c1, botColor = *c2;

    float invSlope1 = (mid.y-top.y) > 0 ? (mid.x-top.x)/(mid.y-top.y) : 0;
    float invSlope2 = (bot.y-top.y) > 0 ? (bot.x-top.x)/(bot.y-top.y) : 0;
    float invSlope3 = (bot.y-mid.y) > 0 ? (bot.x-mid.x)/(bot.y-mid.y) : 0;

    // Draw TOP to MID
    float xStart = top.x;
    float xEnd = top.x;
    for (float y=top.y; y<=mid.y; ++y) {
        float t1 = (y-top.y)/(mid.y-top.y);
        float t2 = (y-top.y)/(bot.y-top.y);

        math::vec3 colorStart = math::linInterpolVec3(t1, midColor, topColor);
        math::vec3 colorEnd = math::linInterpolVec3(t2, botColor, topColor);

        if (xStart > xEnd) { swap(xStart, xEnd); swap(colorStart, colorEnd); }
        for (int x=static_cast<int>(xStart); x<=static_cast<int>(xEnd); ++x) {
            float t = (x-xStart)/(xEnd-xStart);
            math::vec3 c = math::linInterpolVec3(t, colorEnd, colorStart);
            display.setPixel(x, y, c);
        }
        xStart += invSlope1;
        xEnd += invSlope2;
    }

    // Draw MID to BOT
    xStart = mid.x;
    xEnd = top.x + invSlope2 * (mid.y - top.y);
    for (int y=static_cast<int>(mid.y); y<=static_cast<int>(bot.y); ++y) {
        float t1 = (y-mid.y)/(bot.y-mid.y);
        float t2 = (y-top.y)/(bot.y-top.y);

        math::vec3 colorStart = math::linInterpolVec3(t1, botColor, midColor);
        math::vec3 colorEnd = math::linInterpolVec3(t2, botColor, topColor);

        if (xStart > xEnd) { swap(xStart, xEnd); swap(colorStart, colorEnd); }
        for (int x=static_cast<int>(xStart); x<=static_cast<int>(xEnd); ++x) {
            float t = (x-xStart)/(xEnd-xStart);
            math::vec3 color = math::linInterpolVec3(t, colorEnd, colorStart);
            display.setPixel(x, y, color);
        }
        xStart += invSlope3;
        xEnd += invSlope2;
    }
}