#ifndef TRIANGLE
#define TRIANGLE

#include "math.h"

namespace modelling {

    struct Triangle {
        math::vec3 pos[3];
        math::vec3 color[3];

        void setVertex(int i, const math::vec3& pPos, const math::vec3& pColor);
        math::vec3& getVertexPos(int i);
        math::vec3& getVertexColor(int i);

        math::vec3 getNormal();
    };

    int clipTriangleByPlane(modelling::Triangle& triangle, const math::vec3& planePos, const math::vec3& planeNormal, modelling::Triangle& out_tri1, modelling::Triangle& out_tri2);

}

#endif