#include "modelling/mesh.h"

#include <string>
#include <fstream>
#include <sstream>

#include "math.h"
#include "modelling/triangle.h"

namespace modelling {

    Mesh::Mesh() {}

    Mesh::Mesh(const std::string& filepath) {
        fromObjFile(filepath);
    }

    void Mesh::addTriangle(Triangle& t) {
        triangles.push_back(t);
    }

    const std::vector<Triangle>& Mesh::getTriangles() {
        return triangles;
    };

    bool Mesh::fromObjFile(const std::string& filepath) {
        math::vec3 color(128.0f, 128.0f, 128.0f);

        std::ifstream file(filepath);
        if (!file.is_open()) return false;

        std::vector<math::vec3> vertices;
        std::vector<Triangle> faces;

        std::string line;
        std::string prefix;
        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#') continue;

            std::istringstream iss(line);
            iss >> prefix;
            if (prefix == "v") {
                math::vec3 v;
                iss >> v.x >> v.y >> v.z;
                vertices.push_back(v);
            } else if (prefix == "f") {
                Triangle f;
                int a, b, c;
                iss >> a >> b >> c;
                f.setVertex(0, vertices[a-1], color);
                f.setVertex(1, vertices[b-1], color);
                f.setVertex(2, vertices[c-1], color);
                faces.push_back(f);
            }
        }

        for (Triangle& f : faces) {
            addTriangle(f);
        }

        file.close();
        return true;
    }

}