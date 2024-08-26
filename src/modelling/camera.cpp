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

    math::mat4x4 Camera::getProjectionMatrix() {
        float range = tan(math::asRadians(fov/2.0)) * near;
        float left = -range * aspect;
        float right = range * aspect;
        float bottom = -range;
        float top = range;

        return math::mat4x4(
            math::vec4((2.0*near)/(right-left), 0.0, 0.0, 0.0),
            math::vec4(0.0, (2.0*near)/(top-bottom), 0.0, 0.0),
            math::vec4(0.0, 0.0, -(far+near)/(far-near), -(2.0*far*near)/(far-near)),
            math::vec4(0.0, 0.0, -1.0, 0.0)
        );
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