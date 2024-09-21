#include <gtest/gtest.h>

#include "math.h"

TEST(MultThreeByThreeOnThree, NormalCase) {
    math::mat3x3 m(
        math::vec3(1.0f, 4.0f, 7.0f),
        math::vec3(2.0f, 5.0f, 8.0f),
        math::vec3(3.0f, 6.0f, 9.0f)
    );
    math::vec3 v(2.0f, 3.0f, 4.0f);
    math::vec3 r = math::multMat3x3OnVec3(m, v);
    EXPECT_EQ(r.x, 20.0f);
    EXPECT_EQ(r.y, 47.0f);
    EXPECT_EQ(r.z, 74.0f);
}

TEST(MultFourByFourOnFour, NormalCase) {
    math::mat4x4 m(
        math::vec4(1.0f, 5.0f,  9.0f, 13.0f),
        math::vec4(2.0f, 6.0f, 10.0f, 14.0f),
        math::vec4(3.0f, 7.0f, 11.0f, 15.0f),
        math::vec4(4.0f, 8.0f, 12.0f, 16.0f)
    );
    math::vec4 v(2.0f, 3.0f, 4.0f, 5.0f);
    math::vec4 r = math::multMat4x4OnVec4(m, v);
    EXPECT_EQ(r.x, 40.0f);
    EXPECT_EQ(r.y, 96.0f);
    EXPECT_EQ(r.z, 152.0f);
    EXPECT_EQ(r.w, 208.0f);
}

TEST(MultThreeByThreeOnThreeByThree, NormalCase) {
    math::mat3x3 m1(
        math::vec3(1.0f, 4.0f, 7.0f),
        math::vec3(2.0f, 5.0f, 8.0f),
        math::vec3(3.0f, 6.0f, 9.0f)
    );
    math::mat3x3 m2(
        math::vec3(9.0f, 8.0f, 7.0f),
        math::vec3(6.0f, 5.0f, 4.0f),
        math::vec3(3.0f, 2.0f, 1.0f)
    );
    math::mat3x3 r = math::multMat3x3OnMat3x3(m1, m2);
    EXPECT_EQ(r.a.x,  46.0f);
    EXPECT_EQ(r.a.y, 118.0f);
    EXPECT_EQ(r.a.z, 190.0f);
    EXPECT_EQ(r.b.x,  28.0f);
    EXPECT_EQ(r.b.y,  73.0f);
    EXPECT_EQ(r.b.z, 118.0f);
    EXPECT_EQ(r.c.x,  10.0f);
    EXPECT_EQ(r.c.y,  28.0f);
    EXPECT_EQ(r.c.z,  46.0f);
}

TEST(MultFourByFourOnFourByFour, NormalCase) {
    math::mat4x4 m1(
        math::vec4(1.0f, 5.0f,  9.0f, 13.0f),
        math::vec4(2.0f, 6.0f, 10.0f, 14.0f),
        math::vec4(3.0f, 7.0f, 11.0f, 15.0f),
        math::vec4(4.0f, 8.0f, 12.0f, 16.0f)
    );
    math::mat4x4 m2(
        math::vec4(16.0f, 15.0f, 14.0f, 13.0f),
        math::vec4(12.0f, 11.0f, 10.0f,  9.0f),
        math::vec4( 8.0f,  7.0f,  6.0f,  5.0f),
        math::vec4( 4.0f,  3.0f,  2.0f,  1.0f)
    );
    math::mat4x4 r = math::multMat4x4OnMat4x4(m1, m2);
    EXPECT_EQ(r.a.x, 140.0f);
    EXPECT_EQ(r.a.y, 372.0f);
    EXPECT_EQ(r.a.z, 604.0f);
    EXPECT_EQ(r.a.w, 836.0f);
    EXPECT_EQ(r.b.x, 100.0f);
    EXPECT_EQ(r.b.y, 268.0f);
    EXPECT_EQ(r.b.z, 436.0f);
    EXPECT_EQ(r.b.w, 604.0f);
    EXPECT_EQ(r.c.x, 60.0f);
    EXPECT_EQ(r.c.y, 164.0f);
    EXPECT_EQ(r.c.z, 268.0f);
    EXPECT_EQ(r.c.w, 372.0f);
    EXPECT_EQ(r.d.x, 20.0f);
    EXPECT_EQ(r.d.y, 60.0f);
    EXPECT_EQ(r.d.z, 100.0f);
    EXPECT_EQ(r.d.w, 140.0f);
}
