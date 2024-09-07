#include "modelling/scene.h"

Scene::Scene(modelling::Camera& c, math::vec3& ambientLight) : camera(c), ambientLight(ambientLight) {}

void Scene::addMesh(modelling::Mesh& mesh) {
    meshes.push_back(mesh);
}

const std::vector<modelling::Mesh>& Scene::getMeshes() {
    return meshes;
}

modelling::Camera& Scene::getCamera() {
    return camera;
}

math::vec3& Scene::getAmbientLight() {
    return ambientLight;
}