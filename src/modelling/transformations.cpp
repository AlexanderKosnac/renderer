#include "modelling/transformations.h"

#include <cmath>

#include "math.h"

namespace transformation {

    math::mat3x3 rotationX(float degree) {
        float rad = math::asRadians(degree);
        float c = cos(rad);
        float s = sin(rad);
        return math::mat3x3(
            math::vec3(1.0f, 0.0f, 0.0f),
            math::vec3(0.0f,    c,    s),
            math::vec3(0.0f,   -s,    c)
        );
    }

    math::mat3x3 rotationY(float degree) {
        float rad = math::asRadians(degree);
        float c = cos(rad);
        float s = sin(rad);
        return math::mat3x3(
            math::vec3(   c, 0.0f,   -s),
            math::vec3(0.0f, 1.0f, 0.0f),
            math::vec3(   s, 0.0f,    c)
        );
    }

    math::mat3x3 rotationZ(float degree) {
        float rad = math::asRadians(degree);
        float c = cos(rad);
        float s = sin(rad);
        return math::mat3x3(
            math::vec3(   c,    s, 0.0f),
            math::vec3(  -s,    c, 0.0f),
            math::vec3(0.0f, 0.0f, 1.0f)
        );
    }

    math::mat4x4 translate(float dx, float dy, float dz) {
        return math::mat4x4(
            math::vec4(1.0f, 0.0f, 0.0f, 0.0f),
            math::vec4(0.0f, 1.0f, 0.0f, 0.0f),
            math::vec4(0.0f, 0.0f, 1.0f, 0.0f),
            math::vec4(  dx,   dy,   dz, 1.0f)
        );
    }

    math::mat3x3 scale(float m) {
        return scaleXYZ(m, m, m);
    }

    math::mat3x3 scaleXYZ(float x, float y, float z) {
        return math::mat3x3(
            math::vec3(   x, 0.0f, 0.0f),
            math::vec3(0.0f,    y, 0.0f),
            math::vec3(0.0f, 0.0f,    z)
        );
    }

}