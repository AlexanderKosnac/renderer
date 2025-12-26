#include "rasterizer/rasterizer.h"

#include <algorithm>
#include <list>

#include "math.h"
#include "display/x11display.h"
#include "display/framebuffer.h"
#include "modelling/camera.h"
#include "modelling/scene.h"
#include "modelling/transformations.h"

Rasterizer::Rasterizer(Framebuffer& fb, Scene& scene) : framebuffer(fb), scene(scene) {
    updateProjectionMatrix();
}

void Rasterizer::render() {
    math::vec3& pos = scene.getCamera().pos;

    float width2 = 0.5f * framebuffer.width;
    float height2 = 0.5f * framebuffer.height;

    std::vector<modelling::Triangle> depthClippedTriangles;

    // Render all objects in the scene.
    for (auto object : scene.getObjects()) {
        // For each object, each triangle has to be rendered.
        for (auto triangle : object.mesh.getTriangles()) {
            modelling::Triangle transformed;
            for (auto i : { 0, 1, 2 }) {
                math::vec4 v = triangle.pos[i].toVec4(1);
                // Transform each vertex from model to world coordinates by applying all transformation of the object.
                for (auto& transformation : object.modelTransformations) {
                    v = math::multMat4x4OnVec4(transformation, v);
                }
                transformed.pos[i] = v.toVec3();
                transformed.color[i] = triangle.color[i];
            }

            math::vec3 normal = transformed.getNormal();

            // Back culling - do not render triangles that have their back turned to the camera.
            math::vec3 cam(transformed.pos[0].x-pos.x, transformed.pos[0].y-pos.y, transformed.pos[0].z-pos.z);
            if (math::dotVec3(normal, cam) > 0.0f) continue;

            // Calculate ambient lighting intensity based on the angle of the normal to the light direction.
            float min = 0.05;
            float val = -math::dotVec3(normal, scene.getAmbientLight());
            float sim = min + ((val+1.0f)/2.0f) * (1.0f-min); // [-1; 1] -> [min; 1]

            for (auto i : { 0, 1, 2 }) {
                transformed.color[i].x *= sim;
                transformed.color[i].y *= sim;
                transformed.color[i].z *= sim;

                transformed.pos[i] = math::multMat4x4OnVec4(scene.getCamera().viewMatrix, transformed.pos[i].toVec4(1)).dehomogenize();
            }

            // Depth Clipping - clip triangles by the near plane.
            // Triangles that are very close to the camera, become very large causing performance issues. Because of
            // this they are clipped.
            modelling::Triangle t[2];
            int newTriangles = modelling::clipTriangleByPlane(transformed, math::vec3(0.0f, 0.0f, 0.1f), math::vec3(0.0f, 0.0f, 1.0f), t[0], t[1]);

            // The result of the clipping is not the original triangle, but potentially up to two subtriangles, making
            // up the original triangle. The following loop handles each of these triangles.
            for (int i=0; i<newTriangles; i++) {
                modelling::Triangle tri = t[i];
                for (auto i : { 0, 1, 2 }) {
                    // Apply projection, transforming every point to be within a [-1; 1] frustum.
                    tri.pos[i] = math::multMat4x4OnVec4(projectionMatrix, tri.pos[i].toVec4(1)).dehomogenize();

                    // Stretch x and y part to match window size (results are discreet pixel coordinates).
                    // This turns it into a space of [0; screen width], [0; screen height]; [0; 1].
                    tri.pos[i].x = (-tri.pos[i].x + 1.0f) * width2;
                    tri.pos[i].y = (-tri.pos[i].y + 1.0f) * height2;
                }
                depthClippedTriangles.push_back(tri);
            }
        }
    }

    // For each triangle the discreet pixel coordinates on the screen are now known and it is ready to draw. Before
    // that, each triangle has to be clipped along all four edges of the screen.
    for (modelling::Triangle& triangle : depthClippedTriangles) {
        std::list<modelling::Triangle> trianglesToDraw;

        modelling::Triangle t[2];

        trianglesToDraw.push_back(triangle);
        int nNewTriangles = 1;

        // Clip along each screen edge.
        for (int p=0; p<4; p++) {
            int nTrisToAdd = 0;
            while (nNewTriangles > 0) {
                modelling::Triangle candidate = trianglesToDraw.front();
                trianglesToDraw.pop_front();
                nNewTriangles--;

                switch (p) {
                    case 0: nTrisToAdd = modelling::clipTriangleByPlane(candidate, math::vec3(0.0f, 0.0f, 0.0f),                 math::vec3( 0.0f,  1.0f, 0.0f), t[0], t[1]); break; // Right
                    case 1: nTrisToAdd = modelling::clipTriangleByPlane(candidate, math::vec3(0.0f, framebuffer.height-1, 0.0f), math::vec3( 0.0f, -1.0f, 0.0f), t[0], t[1]); break; // Top
                    case 2: nTrisToAdd = modelling::clipTriangleByPlane(candidate, math::vec3(0.0f, 0.0f, 0.0f),                 math::vec3( 1.0f,  0.0f, 0.0f), t[0], t[1]); break; // Left
                    case 3: nTrisToAdd = modelling::clipTriangleByPlane(candidate, math::vec3(framebuffer.width-1, 0.0f, 0.0f),  math::vec3(-1.0f,  0.0f, 0.0f), t[0], t[1]); break; // Bottom
                }

                // Store sub triangles created by clipping process.
                for (int w=0; w<nTrisToAdd; w++) {
                    trianglesToDraw.push_back(t[w]);
                }
            }
            nNewTriangles = trianglesToDraw.size();
        }

        // Triangle vertices are now all within the screen and can be drawn by applying 2D rasterization between them.
        for (modelling::Triangle& t : trianglesToDraw) {
            fillTriangle(t);
            //drawTriangle(t); // Draw wireframe of scene
        }
    }
}

void Rasterizer::updateProjectionMatrix() {
    scene.getCamera().setProjectionMatrix(projectionMatrix);
}

/*
 * Draws a line between two 2D points. Uses Bresenham's line algorithm.
 */
void Rasterizer::drawLine(float x1, float y1, math::vec3& color1, float x2, float y2, math::vec3& color2) {
    const float LINE_Z = 0.0f;

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

        framebuffer.setPixel(x, y, LINE_Z, colorB);

        for (int i=0; x<xe; i++) {
            x += 1;
            if (px < 0) {
                px = px + 2 * dy1;
            } else {
                ((dx<0 && dy<0) || (dx>0 && dy>0)) ? y++ : y--;
                px = px + 2 * (dy1 - dx1);
            }
            math::vec3 color = math::linInterpolVec3((float)i/(float)dx1, colorA, colorB);
            framebuffer.setPixel(x, y, LINE_Z, color);
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

        framebuffer.setPixel(x, y, LINE_Z, colorB);

        for (int i=0; y<ye; i++) {
            y += 1;
            if (py > 0) {
                ((dx<0 && dy<0) || (dx>0 && dy>0)) ? x++ : x--;
                py = py + 2 * (dx1 - dy1);
            } else {
                py = py + 2 * dx1;
            }
            math::vec3 color = math::linInterpolVec3((float)i/(float)dy1, colorA, colorB);
            framebuffer.setPixel(x, y, LINE_Z, color);
        }
    }
}

/*
 * Draws a triangle wire frame by simply drawing three lines to connect the vertices.
 */
void Rasterizer::drawTriangle(modelling::Triangle& t) {
    math::vec3& v0 = t.pos[0];
    math::vec3& v1 = t.pos[1];
    math::vec3& v2 = t.pos[2];

    drawLine(v0.x, v0.y, t.color[0], v1.x, v1.y, t.color[1]);
    drawLine(v1.x, v1.y, t.color[1], v2.x, v2.y, t.color[2]);
    drawLine(v2.x, v2.y, t.color[2], v0.x, v0.y, t.color[0]);
}

/*
 * Triangle rasterization using barycentric coordinates.
 * For this, the barycentric coordinates are calculated for all pixels within a box around the triangle and used to
 * determine whether or not the pixel is located within the triangle.
 * Relyable but expensive implementation.
 */
void fillTriangleBarycentric(Framebuffer& framebuffer, modelling::Triangle& t) {
    const math::vec3 v0 = t.pos[0];
    const math::vec3 v1 = t.pos[1];
    const math::vec3 v2 = t.pos[2];
    const math::vec3 c0 = t.color[0];
    const math::vec3 c1 = t.color[1];
    const math::vec3 c2 = t.color[2];

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
                framebuffer.setPixel(x, y, z, color);
            }
        }
    }
}

/*
 * Triangle rasterization using the scan line algorithm.
 * For this the vertices are sorted by their y-coordinate and then the triangle is rendered in two parts. From the top
 * to the middle part, and then from the middle to the bottom.
 */
void fillTriangleScanLine(Framebuffer& framebuffer, modelling::Triangle& t) {
    math::vec3 c;
    math::vec3 v0 = t.pos[0];
    math::vec3 v1 = t.pos[1];
    math::vec3 v2 = t.pos[2];
    math::vec3 c0 = t.color[0];
    math::vec3 c1 = t.color[1];
    math::vec3 c2 = t.color[2];

    // Vertices sorted by Y
    if (v1.y < v0.y) { std::swap(v0, v1); std::swap(c0, c1); }
    if (v2.y < v0.y) { std::swap(v0, v2); std::swap(c0, c2); }
    if (v2.y < v1.y) { std::swap(v1, v2); std::swap(c1, c2); }

    float invSlope1 = ((v1.y-v0.y == 0)) ? 0.0f : (v1.x-v0.x)/(v1.y-v0.y);
    float invSlope2 = ((v2.y-v0.y == 0)) ? 0.0f : (v2.x-v0.x)/(v2.y-v0.y);

    // Top to middle
    for (float y=v0.y; y<v1.y; y++) {
        float t1 = (y-v0.y)/(v1.y-v0.y);
        float t2 = (y-v0.y)/(v2.y-v0.y);

        int xStart = static_cast<int>(v0.x + (y-v0.y) * invSlope1);
        int xEnd = static_cast<int>(v0.x + (y-v0.y) * invSlope2);

        if (xStart > xEnd) std::swap(xStart, xEnd);

        float zStart = math::linInterpolFloat(t1, v0.z, v1.z);
        float zEnd = math::linInterpolFloat(t2, v0.z, v2.z);

        math::vec3 colorStart = math::linInterpolVec3(t1, c0, c1);
        math::vec3 colorEnd = math::linInterpolVec3(t2, c0, c2);

        for (int x=xStart; x<=xEnd; x++) {
            float t = (xStart == xEnd) ? 1.0f : static_cast<float>(x-xStart)/(xEnd-xStart);
            float z = math::linInterpolFloat(t, zStart, zEnd);
            c = math::linInterpolVec3(t, colorStart, colorEnd);
            framebuffer.setPixel(x, static_cast<int>(y), z, c);
        }
    }

    if (v2.y-v1.y != 0) invSlope1 = (v2.x-v1.x)/(v2.y-v1.y);

    // Middle to bottom
    for (float y=v1.y; y<v2.y; y++) {
        float t1 = (y-v1.y)/(v2.y-v1.y);
        float t2 = (y-v0.y)/(v2.y-v0.y);

        int xStart = static_cast<int>(v1.x + (y-v1.y) * invSlope1);
        int xEnd = static_cast<int>(v0.x + (y-v0.y) * invSlope2);
        if (xStart > xEnd) std::swap(xStart, xEnd);

        float zStart = math::linInterpolFloat(t1, v1.z, v2.z);
        float zEnd = math::linInterpolFloat(t2, v0.z, v2.z);

        math::vec3 colorStart = math::linInterpolVec3(t1, c1, c2);
        math::vec3 colorEnd = math::linInterpolVec3(t2, c0, c2);

        for (int x=xStart; x<=xEnd; x++) {
            float t = (xStart == xEnd) ? 1.0f : static_cast<float>(x-xStart)/(xEnd-xStart);
            float z = math::linInterpolFloat(t, zStart, zEnd);
            c = math::linInterpolVec3(t, colorStart, colorEnd);
            framebuffer.setPixel(x, static_cast<int>(y), z, c);
        }
    }
}

void Rasterizer::fillTriangle(modelling::Triangle& t) {
    //fillTriangleBarycentric(framebuffer, t);
    fillTriangleScanLine(framebuffer, t);
}