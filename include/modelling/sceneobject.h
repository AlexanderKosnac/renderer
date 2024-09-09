#ifndef SCENE_OBJECT
#define SCENE_OBJECT

#include <vector>

#include "math.h"
#include "modelling/mesh.h"

namespace modelling {

    struct SceneObject {
        modelling::Mesh mesh;
        std::vector<math::mat4x4> modelTransformations;
    };

}

#endif