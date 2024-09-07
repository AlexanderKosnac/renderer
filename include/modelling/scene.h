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
    math::vec3 ambientLight;

public:
    Scene(modelling::Camera& c, math::vec3& ambientLight);

    void addMesh(modelling::Mesh& mesh);

    const std::vector<modelling::Mesh>& getMeshes();
    modelling::Camera& getCamera();
    math::vec3& getAmbientLight();
};

#endif