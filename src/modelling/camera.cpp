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

    math::vec3& Camera::getPos() {
        return pos;
    }

    math::vec3 Camera::normedU() {
        return math::normVec3(u);
    }

    math::vec3 Camera::normedV() {
        return math::normVec3(v);
    }

    math::vec3 Camera::normedW() {
        return math::normVec3(w);
    }
}