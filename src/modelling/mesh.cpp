#include "modelling/mesh.h"

namespace modelling {

    Mesh::Mesh() {}

    void Mesh::addTriangle(Triangle& t) {
        triangles.push_back(t);
    }

    const std::vector<Triangle>& Mesh::getTriangles() {
        return triangles;
    };

}