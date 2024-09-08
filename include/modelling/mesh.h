#ifndef MESH
#define MESH

#include <vector>
#include <string>

#include "math.h"
#include "modelling/triangle.h"

namespace modelling {

    class Mesh {
    private:
        std::vector<Triangle> triangles;

    public:
        Mesh();
        Mesh(const std::string& filepath);

        void addTriangle(Triangle& t);

        const std::vector<Triangle>& getTriangles();

        bool fromObjFile(const std::string& filepath);
    };

}

#endif