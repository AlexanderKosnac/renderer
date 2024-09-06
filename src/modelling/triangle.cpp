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

    math::vec3 Triangle::getNormal() {
        math::vec3 l1(
            pos[1].x - pos[0].x,
            pos[1].y - pos[0].y,
            pos[1].z - pos[0].z
        );
        math::vec3 l2(
            pos[2].x - pos[0].x,
            pos[2].y - pos[0].y,
            pos[2].z - pos[0].z
        );
        math::vec3 c = math::crossVec3(l1, l2);
        return math::normVec3(c);
    }
}
