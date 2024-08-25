#include "modelling/scene.h"

Scene::Scene(modelling::Camera& c) : camera(c) {}

void Scene::addMesh(modelling::Mesh& mesh) {
    meshes.push_back(mesh);
}

const std::vector<modelling::Mesh>& Scene::getMeshes() {
    return meshes;
}