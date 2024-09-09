#include "math.h"

namespace transformation {

    math::mat3x3 rotationX(float degree);
    math::mat3x3 rotationY(float degree);
    math::mat3x3 rotationZ(float degree);

    math::mat4x4 translate(float dx, float dy, float dz);

    math::mat3x3 scaleXYZ(float x, float y, float z);

}