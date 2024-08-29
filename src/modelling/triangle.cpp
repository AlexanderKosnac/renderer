#include "modelling/triangle.h"

#include "math.h"

namespace modelling {

    void Triangle::setVertex(int i, const math::vec3& pPos, const math::vec3& pColor) {
        pos[i] = pPos;
        color[i] = pColor;
    }

    math::vec3& Triangle::getVertexPos(int i) {
        return pos[i];
    }

    math::vec3& Triangle::getVertexColor(int i) {
        return color[i];
    }

}
