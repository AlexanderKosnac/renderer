#include "modelling/triangle.h"

#include <algorithm>

namespace modelling {

    void Triangle::setVertex(int i, const math::vec3& pPos, const math::vec3& pColor) {
        pos[i] = pPos;
        color[i] = pColor;
    }

    math::vec3& Triangle::getVertexPos(int i) {
        return pos[i];
    }

    math::vec3& Triangle::getVertexColor(int i) {
        return color[i];
    }

    math::vec3 Triangle::getNormal() {
        math::vec3 l1(
            pos[1].x - pos[0].x,
            pos[1].y - pos[0].y,
            pos[1].z - pos[0].z
        );
        math::vec3 l2(
            pos[2].x - pos[0].x,
            pos[2].y - pos[0].y,
            pos[2].z - pos[0].z
        );
        math::vec3 c = math::crossVec3(l1, l2);
        return math::normVec3(c);
    }

    int clipTriangleByPlane(modelling::Triangle& triangle, const math::vec3& planePos, const math::vec3& planeNormal, modelling::Triangle& out_tri1, modelling::Triangle& out_tri2) {
        math::vec3 normal = normVec3(planeNormal);

        math::vec3* inPoints[3];
        math::vec3* inColors[3];
        int inCount = 0;

        math::vec3* outPoints[3];
        math::vec3* outColors[3];
        int outCount = 0;

        for (auto i : { 0, 1, 2 }) {
            math::vec3& p = triangle.pos[i];
            math::vec3& c = triangle.color[i];
            bool isInside = math::dotVec3(math::vec3(p.x-planePos.x, p.y-planePos.y, p.z-planePos.z), normal) >= 0;
            if (isInside) {
                inPoints[inCount] = &p;
                inColors[inCount] = &c;
                inCount++;
            } else {
                outPoints[outCount] = &p;
                outColors[outCount] = &c;
                outCount++;
            }
        }

        if (inCount == 0) { // Triangle completely outside
            return 0;
        }

        if (inCount == 3) { // Triangle completely inside
            out_tri1 = triangle;
            return 1;
        }

        if (inCount == 1 && outCount == 2) { // Triangle "legs" outside
            float t0, t1;
            out_tri1.pos[0] = *inPoints[0];
            out_tri1.pos[1] = math::vectorPlaneIntersection(planePos, normal, *inPoints[0], *outPoints[0], t0);
            out_tri1.pos[2] = math::vectorPlaneIntersection(planePos, normal, *inPoints[0], *outPoints[1], t1);

            out_tri1.color[0] = *inColors[0];
            out_tri1.color[1] = math::linInterpolVec3(t0, *inColors[0], *outColors[0]);
            out_tri1.color[2] = math::linInterpolVec3(t1, *inColors[0], *outColors[1]);

            return 1;
        }

        if (inCount == 2 && outCount == 1) { // Triangle "head" outside
            float t;

            out_tri1.pos[0] = *inPoints[0];
            out_tri1.pos[1] = *inPoints[1];
            out_tri1.pos[2] = math::vectorPlaneIntersection(planePos, normal, *inPoints[0], *outPoints[0], t);

            out_tri1.color[0] = *inColors[0];
            out_tri1.color[1] = *inColors[1];
            out_tri1.color[2] = math::linInterpolVec3(t, *inColors[0], *outColors[0]);

            out_tri2.pos[0] = out_tri1.pos[1];
            out_tri2.pos[1] = out_tri1.pos[2];
            out_tri2.pos[2] = math::vectorPlaneIntersection(planePos, normal, *inPoints[1], *outPoints[0], t);

            out_tri2.color[0] = out_tri1.color[1];
            out_tri2.color[1] = out_tri1.color[2];
            out_tri2.color[2] = math::linInterpolVec3(t, *inColors[1], *outColors[0]);

            return 2;
        }
    }

}
