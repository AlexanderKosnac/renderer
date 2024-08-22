#include "math.h"

namespace math {

    vec3::vec3() : x(0.0f), y(0.0f), z(0.0f) {}

    vec3::vec3(float xx, float yy, float zz) : x(xx), y(yy), z(zz) {}

    vec4::vec4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}

    vec4::vec4(float xx, float yy, float zz, float ww) : x(xx), y(yy), z(zz), w(ww) {}

    mat3x3::mat3x3(vec3 aa, vec3 bb, vec3 cc) : a(aa), b(bb), c(cc) {}

    mat4x4::mat4x4(vec4 aa, vec4 bb, vec4 cc, vec4 dd) : a(aa), b(bb), c(cc), d(dd) {}

    float crossVec3(vec3 vecA, vec3 vecB) {
        return vecA.x * vecB.x + vecA.y * vecB.y + vecA.z * vecB.z;
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
}