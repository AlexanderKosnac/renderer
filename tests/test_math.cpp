#include <gtest/gtest.h>

#include "math.h"

TEST(MatMultThreeByThreeOnThree, NormalCase) {
    math::mat3x3 m(
        math::vec3(1.0f, 4.0f, 7.0f),
        math::vec3(2.0f, 5.0f, 8.0f),
        math::vec3(3.0f, 6.0f, 9.0f)
    );
    math::vec3 v(2.0f, 3.0f, 4.0f);
    math::vec3 r = math::multMat3x3OnVec3(m ,v);
    EXPECT_EQ(r.x, 20.0f);
    EXPECT_EQ(r.y, 47.0f);
    EXPECT_EQ(r.z, 74.0f);
}

TEST(MatMultFourByFourOnFour, NormalCase) {
    math::mat4x4 m(
        math::vec4(1.0f, 5.0f,  9.0f, 13.0f),
        math::vec4(2.0f, 6.0f, 10.0f, 14.0f),
        math::vec4(3.0f, 7.0f, 11.0f, 15.0f),
        math::vec4(4.0f, 8.0f, 12.0f, 16.0f)
    );
    math::vec4 v(2.0f, 3.0f, 4.0f, 5.0f);
    math::vec4 r = math::multMat4x4OnVec4(m ,v);
    EXPECT_EQ(r.x, 40.0f);
    EXPECT_EQ(r.y, 96.0f);
    EXPECT_EQ(r.z, 152.0f);
    EXPECT_EQ(r.w, 208.0f);
}
