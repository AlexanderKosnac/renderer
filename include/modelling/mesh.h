#ifndef MESH
#define MESH

#include <vector>

#include "math.h"
#include "modelling/triangle.h"

namespace modelling {

    class Mesh {
    private:
        std::vector<Triangle> triangles;

    public:
        Mesh();

        void addTriangle(Triangle& t);

        const std::vector<Triangle>& getTriangles();
    };

}

#endif