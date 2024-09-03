#include "modelling/camera.h"

#include "cmath"

#include "math.h"

namespace modelling {

    Camera::Camera(float pFov, float pAspect, float pNear, float pFar, math::vec3 pPos, math::vec3 pUp, math::vec3 pLookat) : fov(pFov), aspect(pAspect), near(pNear), far(pFar), pos(pPos), up(pUp), lookat(pLookat) {
        updateCameraTransformation();
    }

    void Camera::updateCameraTransformation() {
        uvw.a = math::normVec3(math::crossVec3(up, lookat)).toVec4(0);
        uvw.b = math::normVec3(math::crossVec3(lookat, uvw.a.toVec3())).toVec4(0);
        uvw.c = math::normVec3(math::vec3(lookat.x, lookat.y, lookat.z)).toVec4(0);
        uvw.d = pos.toVec4(1);
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

}