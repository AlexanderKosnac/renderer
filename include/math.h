#ifndef MATH
#define MATH

namespace math {

    float asRadians(float degrees);

    struct vec4; // Forward declaration of vec4

    struct vec3 {
        float x, y, z;

        vec3();
        vec3(float xx, float yy, float zz);

        vec4 toVec4(float ww);
    };

    struct vec4 {
        float x, y, z, w;

        vec4();
        vec4(float xx, float yy, float zz, float ww);

        vec3 toVec3();
        vec3 dehomogenize();
    };

    struct mat3x3 {
        vec3 a, b, c;

        mat3x3(vec3 aa, vec3 bb, vec3 cc);
    };

    struct mat4x4 {
        vec4 a, b, c, d;

        mat4x4(vec4 aa, vec4 bb, vec4 cc, vec4 dd);
    };

    vec3 crossVec3(vec3 vecA, vec3 vecB);
    vec3 normVec3(vec3 vec);

    vec3 multMat3x3OnVec3(mat3x3 mat, vec3 vec);
    vec4 multMat4x4OnVec4(mat4x4 mat, vec4 vec);
    mat3x3 multMat3x3OnMat3x3(mat3x3 matA, mat3x3 matB);
    mat4x4 multMat4x4OnMat4x4(mat4x4 matA, mat4x4 matB);
}

#endif