#include "modelling/camera.h"

#include "cmath"

#include "math.h"

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

    math::mat3x3 Camera::getCameraTransformation() {
        return math::mat3x3(pos, lookat, up);
    }

    void Camera::setProjectionMatrix(math::mat4x4& target) {
        float fovRads = math::asRadians(fov);

        target.a.x = aspect*fovRads;
        target.b.y = fovRads;
        target.c.z = far/(far-near);
        target.c.w = 1.0f;
        target.d.z = (-far*near)/(far-near);
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