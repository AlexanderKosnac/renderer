#include "modelling/scene.h"

Scene::Scene(modelling::Camera& c, math::vec3& ambientLight) : camera(c), ambientLight(ambientLight) {}

void Scene::addObject(modelling::Mesh& mesh) {
    objects.push_back(modelling::SceneObject { mesh, std::vector<math::mat4x4>() });
}

void Scene::addObject(modelling::Mesh& mesh, const std::vector<math::mat4x4>& modelTransformation) {
    objects.push_back(modelling::SceneObject { mesh, modelTransformation });
}

const std::vector<modelling::SceneObject>& Scene::getObjects() {
    return objects;
}

modelling::Camera& Scene::getCamera() {
    return camera;
}

math::vec3& Scene::getAmbientLight() {
    return ambientLight;
}