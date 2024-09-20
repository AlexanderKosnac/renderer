#include "math.h"

#include <cmath>
#include <modelling/triangle.h>

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

    mat3x3::mat3x3() : a(vec3()), b(vec3()), c(vec3()) {}

    mat3x3::mat3x3(vec3 aa, vec3 bb, vec3 cc) : a(aa), b(bb), c(cc) {}

    mat3x3::mat3x3(const mat3x3& other) : a(other.a), b(other.b), c(other.c) {}

    mat4x4 mat3x3::toMat4x4() {
        return mat4x4(a.toVec4(0), b.toVec4(0), c.toVec4(0), vec4(0.0f, 0.0f, 0.0f, 1.0f));
    }

    mat4x4::mat4x4() : a(vec4()), b(vec4()), c(vec4()), d(vec4()) {}

    mat4x4::mat4x4(vec4 aa, vec4 bb, vec4 cc, vec4 dd) : a(aa), b(bb), c(cc), d(dd) {}

    mat4x4::mat4x4(const mat4x4& other) : a(other.a), b(other.b), c(other.c), d(other.d) {}

    vec3 crossVec3(vec3& vecA, vec3& vecB) {
        return vec3(
            vecA.y * vecB.z - vecA.z * vecB.y,
            vecA.z * vecB.x - vecA.x * vecB.z,
            vecA.x * vecB.y - vecA.y * vecB.x
        );
    }

    vec3 normVec3(const vec3& vec) {
        float length = std::sqrt(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z);
        return (length > 0.0f) ? vec3(vec.x/length, vec.y/length, vec.z/length) : vec3(0.0f, 0.0f, 0.0f);
    }

    float dotVec3(const vec3& vecA, const vec3& vecB) {
        return vecA.x * vecB.x + vecA.y * vecB.y + vecA.z * vecB.z;
    }

    float dotVec4(const vec4& vecA, const vec4& vecB) {
        return vecA.x * vecB.x + vecA.y * vecB.y + vecA.z * vecB.z + vecA.w * vecB.w;
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
        vec4 r1 = vec4(matA.a.x, matA.b.x, matA.c.x, matA.d.x);
        vec4 r2 = vec4(matA.a.y, matA.b.y, matA.c.y, matA.d.y);
        vec4 r3 = vec4(matA.a.z, matA.b.z, matA.c.z, matA.d.z);
        vec4 r4 = vec4(matA.a.w, matA.b.w, matA.c.w, matA.d.w);

        vec4 c1 = matB.a;
        vec4 c2 = matB.b;
        vec4 c3 = matB.c;
        vec4 c4 = matB.d;

        return mat4x4(
            vec4(dotVec4(r1, c1), dotVec4(r2, c1), dotVec4(r3, c1), dotVec4(r4, c1)),
            vec4(dotVec4(r1, c2), dotVec4(r2, c2), dotVec4(r3, c2), dotVec4(r4, c2)),
            vec4(dotVec4(r1, c3), dotVec4(r2, c3), dotVec4(r3, c3), dotVec4(r4, c3)),
            vec4(dotVec4(r1, c4), dotVec4(r2, c4), dotVec4(r3, c4), dotVec4(r4, c4))
        );
    }

    vec3 linInterpolVec3(float t, const vec3& vecA, const vec3& vecB) {
        return vec3(
            vecA.x + t * (vecB.x - vecA.x),
            vecA.y + t * (vecB.y - vecA.y),
            vecA.z + t * (vecB.z - vecA.z)
        );
    }

    float linInterpolFloat(const float& t, const float& a, const float& b) {
        return a + t * (b - a); // This implementation is more floating point stable than a * (1 - t) + b * t.
    }

    void barycentric(const vec3& v0, const vec3& v1, const vec3& v2, const vec3& p, float& l1, float& l2, float& l3) {
        double denom = (v1.y-v2.y) * (v0.x-v2.x) + (v2.x-v1.x) * (v0.y-v2.y);

        l1 = ((v1.y-v2.y) * (p.x-v2.x) + (v2.x-v1.x) * (p.y-v2.y)) / denom;
        l2 = ((v2.y-v0.y) * (p.x-v2.x) + (v0.x-v2.x) * (p.y-v2.y)) / denom;
        l3 = 1.0 - l1 - l2;
    }

    vec3 vectorPlaneIntersection(const vec3& planePoint, const vec3& planeNormal, const vec3& lineStart, const vec3& lineEnd, float& t) {
        vec3 normal = normVec3(planeNormal);
        float d = -dotVec3(planePoint, normal);
        float ad = dotVec3(lineStart, normal);
        float bd = dotVec3(lineEnd, normal);
        t = (-d-ad)/(bd-ad);
        return vec3(
            (lineEnd.x - lineStart.x) * t + lineStart.x,
            (lineEnd.y - lineStart.y) * t + lineStart.y,
            (lineEnd.z - lineStart.z) * t + lineStart.z
        );
    }

}