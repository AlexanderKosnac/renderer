#include "modelling/camera.h"

#include <cmath>
#include <algorithm>

#include "math.h"
#include "modelling/transformations.h"

namespace modelling {

    Camera::Camera(float pFov, float pAspect, float pNear, float pFar, math::vec3 pPos, math::vec3 pUp, math::vec3 pLookat) : fov(pFov), aspect(pAspect), near(pNear), far(pFar), pos(pPos), up(pUp), lookat(pLookat) {
        defaultLookat = math::vec3(pLookat.x, pLookat.y, pLookat.z);
        updateViewTransformation();
    }

    void Camera::yawLookAt(float degree) {
        yaw = fmod(yaw + degree + 360.0f, 360.0f);
    }

    void Camera::pitchLookAt(float degree) {
        pitch = std::clamp(pitch + degree, -89.0f, 89.0f);
    }

    void Camera::rollLookAt(float degree) {
        roll = std::clamp(roll + degree, -89.0f, 89.0f);
    }

    void Camera::updateViewTransformation() {
        lookat = defaultLookat;
        lookat = math::multMat3x3OnVec3(transformation::rotationX(pitch), lookat);
        lookat = math::multMat3x3OnVec3(transformation::rotationY(yaw), lookat);
        lookat = math::multMat3x3OnVec3(transformation::rotationZ(roll), lookat);

        math::vec3 w(-lookat.x, -lookat.y, -lookat.z);
        math::vec3 nRight = math::crossVec3(up, w);
        math::vec3 nUp = math::crossVec3(w, nRight);

        cameraMatrix.a = math::normVec3(nRight);
        cameraMatrix.b = math::normVec3(nUp);
        cameraMatrix.c = math::normVec3(w);

        viewMatrix.a = math::vec4(cameraMatrix.a.x, cameraMatrix.b.x, cameraMatrix.c.x, 0.0f);
        viewMatrix.b = math::vec4(cameraMatrix.a.y, cameraMatrix.b.y, cameraMatrix.c.y, 0.0f);
        viewMatrix.c = math::vec4(cameraMatrix.a.z, cameraMatrix.b.z, cameraMatrix.c.z, 0.0f);
        viewMatrix.d = math::vec4(-dotVec3(pos, cameraMatrix.a), -dotVec3(pos, cameraMatrix.b), -dotVec3(pos, cameraMatrix.c), 1.0f);
    }

    void Camera::setProjectionMatrix(math::mat4x4& target) {
        float invTanFov = 1/tan(math::asRadians(fov)/2);

        target.a.x = invTanFov/aspect;
        target.b.y = invTanFov;
        target.c.z = (far+near)/(near-far);
        target.c.w = (2*far*near)/(near-far);
        target.d.z = -1.0f;
    }

    math::vec3& Camera::getPos() {
        return pos;
    }

}