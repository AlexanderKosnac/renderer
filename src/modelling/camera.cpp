#include "math.h"
#include "modelling/camera.h"

namespace modelling {

    Camera::Camera(float pFov, float pAspect, float pNear, float pFar, math::vec3 pPos, math::vec3 pUp, math::vec3 pLookat) {
        fov = pFov;
        aspect = pAspect;
        near = pNear;
        far = pFar;
        pos = pPos;
        up = pUp;
        lookat = pLookat;

        u = math::crossVec3(up, lookat);
        v = math::crossVec3(lookat, u);
        w = lookat;
    }

}