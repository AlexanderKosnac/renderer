#include "math.h"

#include <cmath>

namespace math {

    const float PI = 3.141592654;

    float asRadians(float degrees) {
        return degrees*PI/180;
    }

    vec3::vec3() : x(0.0f), y(0.0f), z(0.0f) {}

    vec3::vec3(float xx, float yy, float zz) : x(xx), y(yy), z(zz) {}

    vec4 vec3::toVec4(float ww) {
        return vec4(x, y, z, ww);
    }

    vec4::vec4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}

    vec4::vec4(float xx, float yy, float zz, float ww) : x(xx), y(yy), z(zz), w(ww) {}

    vec3 vec4::toVec3() {
        return vec3(x, y, z);
    }

    vec3 vec4::dehomogenize() {
        return (w == 0.0f) ? vec3(x, y, z) : vec3(x/w, y/w, z/w);
    }

    mat3x3::mat3x3(vec3 aa, vec3 bb, vec3 cc) : a(aa), b(bb), c(cc) {}

    mat4x4::mat4x4(vec4 aa, vec4 bb, vec4 cc, vec4 dd) : a(aa), b(bb), c(cc), d(dd) {}

    vec3 crossVec3(vec3 vecA, vec3 vecB) {
        return vec3(
            vecA.y * vecB.z - vecA.z * vecB.y,
            vecA.z * vecB.x - vecA.x * vecB.z,
            vecA.x * vecB.y - vecA.y * vecB.x
        );
    }

    vec3 normVec3(vec3 vec) {
        float length = std::sqrt(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z);
        return (length > 0.0f) ? vec3(vec.x/length, vec.y/length, vec.z/length) : vec3(0.0f, 0.0f, 0.0f);
    }

    vec3 multMat3x3OnVec3(mat3x3 mat, vec3 vec) {
        return vec3(
            mat.a.x * vec.x + mat.b.x * vec.y + mat.c.x * vec.z,
            mat.a.y * vec.x + mat.b.y * vec.y + mat.c.y * vec.z,
            mat.a.z * vec.x + mat.b.z * vec.y + mat.c.z * vec.z
        );
    }

    vec4 multMat4x4OnVec4(mat4x4 mat, vec4 vec) {
        return vec4(
            mat.a.x * vec.x + mat.b.x * vec.y + mat.c.x * vec.z + mat.d.x * vec.w,
            mat.a.y * vec.x + mat.b.y * vec.y + mat.c.y * vec.z + mat.d.y * vec.w,
            mat.a.z * vec.x + mat.b.z * vec.y + mat.c.z * vec.z + mat.d.z * vec.w,
            mat.a.w * vec.x + mat.b.w * vec.y + mat.c.w * vec.z + mat.d.w * vec.w
        );
    }

    mat3x3 multMat3x3OnMat3x3(mat3x3 matA, mat3x3 matB) {
        return mat3x3(
            multMat3x3OnVec3(matA, vec3(matB.a.x, matB.b.x, matB.c.x)),
            multMat3x3OnVec3(matA, vec3(matB.a.y, matB.b.y, matB.c.y)),
            multMat3x3OnVec3(matA, vec3(matB.a.z, matB.b.z, matB.c.z))
        );
    }

    mat4x4 multMat4x4OnMat4x4(mat4x4 matA, mat4x4 matB) {
        return mat4x4(
            multMat4x4OnVec4(matA, vec4(matB.a.x, matB.b.x, matB.c.x, matB.d.x)),
            multMat4x4OnVec4(matA, vec4(matB.a.y, matB.b.y, matB.c.y, matB.d.y)),
            multMat4x4OnVec4(matA, vec4(matB.a.z, matB.b.z, matB.c.z, matB.d.z)),
            multMat4x4OnVec4(matA, vec4(matB.a.w, matB.b.w, matB.c.w, matB.d.w))
        );
    }

    math::vec3 linInterpolVec3(float t, math::vec3& vecA, math::vec3& vecB) {
        float ti = 1 - t;
        return math::vec3(
            vecA.x * t + vecB.x * ti,
            vecA.y * t + vecB.y * ti,
            vecA.z * t + vecB.z * ti
        );
    }

    void barycentric(const math::vec3& v0, const math::vec3& v1, const math::vec3& v2, const math::vec3& p, float& l1, float& l2, float& l3) {
        double denom = (v1.y-v2.y) * (v0.x-v2.x) + (v2.x-v1.x) * (v0.y-v2.y);

        l1 = ((v1.y-v2.y) * (p.x-v2.x) + (v2.x-v1.x) * (p.y-v2.y)) / denom;
        l2 = ((v2.y-v0.y) * (p.x-v2.x) + (v0.x-v2.x) * (p.y-v2.y)) / denom;
        l3 = 1.0 - l1 - l2;
    }
}