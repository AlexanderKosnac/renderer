#ifndef SCENE
#define SCENE

#include <vector>

#include "math.h"
#include "modelling/triangle.h"
#include "modelling/mesh.h"
#include "modelling/camera.h"

class Scene {
private:
    std::vector<modelling::Mesh> meshes;
    modelling::Camera camera;

public:
    Scene(modelling::Camera& c);

    void addMesh(modelling::Mesh& mesh);

    const std::vector<modelling::Mesh>& getMeshes();
    modelling::Camera& getCamera();
};

#endif