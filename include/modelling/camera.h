#ifndef CAMERA
#define CAMERA

#include "math.h"

namespace modelling {

    struct Camera {
        float fov, aspect, near, far;
        math::vec3 pos, up, lookat;

        math::vec3 defaultLookat;

        math::mat3x3 cameraMatrix;
        math::mat4x4 viewMatrix;

        float yaw = 0, pitch = 0, roll = 0;

        Camera(float pFov, float pAspect, float pNear, float pFar, math::vec3 pPos, math::vec3 pUp, math::vec3 pLookat);

        void yawLookAt(float degree);
        void pitchLookAt(float degree);
        void rollLookAt(float degree);

        void updateViewTransformation();
        void setProjectionMatrix(math::mat4x4& target);

        math::vec3& getPos();
    };

}

#endif