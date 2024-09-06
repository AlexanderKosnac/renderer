#ifndef CAMERA
#define CAMERA

#include "math.h"

namespace modelling {

    struct Camera {
        float fov, aspect, near, far;
        math::vec3 pos, up, lookat;

        math::mat3x3 cameraMatrix;
        math::mat4x4 viewMatrix;

        Camera(float pFov, float pAspect, float pNear, float pFar, math::vec3 pPos, math::vec3 pUp, math::vec3 pLookat);

        void rotateLookAt(int yaw, int pitch, int roll);
        void updateViewTransformation();
        void setProjectionMatrix(math::mat4x4& target);

        math::vec3& getPos();
    };

}

#endif