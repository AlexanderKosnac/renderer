#include "modelling/triangle.h"

namespace modelling {

    void Triangle::setVertex(int i, const math::vec3& pPos, const math::vec3& pColor) {
        pos[i] = pPos;
        color[i] = pColor;
    }

}
