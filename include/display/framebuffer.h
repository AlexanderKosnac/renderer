#ifndef FRAMEBUFFER
#define FRAMEBUFFER

#include <vector>
#include <algorithm>

#include "math.h"

struct Framebuffer {
    int width;
    int height;

    std::vector<unsigned char> color; // RGBA, 8 bits per channel
    std::vector<float> zbuffer; // Depth buffer, 0 = near, 1 = far

    Framebuffer(int w, int h) : width(w), height(h), color(w * h * 4), zbuffer(w * h, 1.0f) {}

    void clear() {
        std::fill(color.begin(), color.end(), 0);
    }

    void clearZ() {
        std::fill(zbuffer.begin(), zbuffer.end(), 1.0f);
    }

    void setPixel(int x, int y, float z, const math::vec3& c) {
        if (x < 0 || y < 0 || x >= width || y >= height)
            return;

        const int i = y * width + x;

        if (z > zbuffer[i]) // Smaller z = closer
            return;

        zbuffer[i] = z;

        const int ii = i * 4;
        color[ii + 0] = static_cast<unsigned char>(c.z);
        color[ii + 1] = static_cast<unsigned char>(c.y);
        color[ii + 2] = static_cast<unsigned char>(c.x);
        color[ii + 3] = 255;
    }
};

#endif