#ifndef TRIANGLE
#define TRIANGLE

#include "math.h"

namespace modelling {

    struct Triangle {
        math::vec3 pos[3];
        math::vec3 color[3];

        void setVertex(int i, const math::vec3& pPos, const math::vec3& pColor);
    };

}

#endif