#ifndef CAMERA
#define CAMERA

#include "math.h"

namespace modelling {

    struct Camera {
        float fov, aspect, near, far;
        math::vec3 pos, up, lookat;

        math::mat4x4 uvw; // Camera Transformation

        Camera(float pFov, float pAspect, float pNear, float pFar, math::vec3 pPos, math::vec3 pUp, math::vec3 pLookat);

        void updateCameraTransformation();
        void setProjectionMatrix(math::mat4x4& target);

        math::vec3& getPos();
    };

}

#endif