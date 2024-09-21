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

    struct mat4x4; // Forward declaration of mat4x4

    struct mat3x3 {
        vec3 a, b, c;

        mat3x3();
        mat3x3(vec3 aa, vec3 bb, vec3 cc);
        mat3x3(const mat3x3& other);

        mat4x4 toMat4x4();
    };

    struct mat4x4 {
        vec4 a, b, c, d;

        mat4x4();
        mat4x4(vec4 aa, vec4 bb, vec4 cc, vec4 dd);
        mat4x4(const mat4x4& other);
    };

    vec3 crossVec3(vec3& vecA, vec3& vecB);
    vec3 normVec3(const vec3& vec);

    float dotVec3(const vec3& vecA, const vec3& vecB);
    float dotVec4(const vec4& vecA, const vec4& vecB);

    vec3 multMat3x3OnVec3(const mat3x3& mat, const vec3& vec);
    vec4 multMat4x4OnVec4(const mat4x4& mat, const vec4& vec);
    mat3x3 multMat3x3OnMat3x3(const mat3x3& matA, const mat3x3& matB);
    mat4x4 multMat4x4OnMat4x4(const mat4x4& matA, const mat4x4& matB);

    vec3 linInterpolVec3(float t, const vec3& vecA, const vec3& vecB);
    float linInterpolFloat(const float& t, const float& a, const float& b);

    void barycentric(const math::vec3& v0, const math::vec3& v1, const math::vec3& v2, const math::vec3& p, float& l1, float& l2, float& l3);

    vec3 vectorPlaneIntersection(const vec3& planePoint, const vec3& planeNormal, const vec3& lineStart, const vec3& lineEnd, float& t);

}

#endif