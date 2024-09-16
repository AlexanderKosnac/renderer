#ifndef SCENE
#define SCENE

#include <vector>

#include "math.h"
#include "modelling/triangle.h"
#include "modelling/mesh.h"
#include "modelling/camera.h"
#include "modelling/sceneobject.h"

class Scene {
private:
    std::vector<modelling::SceneObject> objects;
    modelling::Camera camera;
    math::vec3 ambientLight;

public:
    Scene(modelling::Camera& c, math::vec3& ambientLight);

    void addObject(const modelling::Mesh& mesh);
    void addObject(const modelling::Mesh& mesh, const std::vector<math::mat4x4>& modelTransformation);

    const std::vector<modelling::SceneObject>& getObjects();
    modelling::Camera& getCamera();
    math::vec3& getAmbientLight();
};

#endif