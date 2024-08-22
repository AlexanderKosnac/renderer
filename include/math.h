#ifndef MATH
#define MATH

namespace math {
    struct vec3 {
        float x, y, z;

        vec3();
        vec3(float xx, float yy, float zz);
    };

    struct vec4 {
        float x, y, z, w;

        vec4();
        vec4(float xx, float yy, float zz, float ww);
    };

    struct mat3x3 {
        vec3 a, b, c;

        mat3x3(vec3 aa, vec3 bb, vec3 cc);
    };

    struct mat4x4 {
        vec4 a, b, c, d;

        mat4x4(vec4 aa, vec4 bb, vec4 cc, vec4 dd);
    };

    float crossVec3(vec3 vecA, vec3 vecB);

    vec3 multMat3x3OnVec3(mat3x3 mat, vec3 vec);
    vec4 multMat4x4OnVec4(mat4x4 mat, vec4 vec);
    mat3x3 multMat3x3OnMat3x3(mat3x3 matA, mat3x3 matB);
    mat4x4 multMat4x4OnMat4x4(mat4x4 matA, mat4x4 matB);
}

#endif