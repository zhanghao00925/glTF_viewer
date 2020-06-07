#pragma once

#include "render_core.h"
#include "mesh.h"

// ASSIMP
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model {
public:
    Model(const string &path);
    void Draw(Shader &shader, mat4 &model, mat4 &view, mat4 &projection);
    ~Model() {
        for (auto &mesh : meshVector) {
            mesh.Release();
        }
    }

    map<string, Mesh> meshMap;
    vector<Mesh> meshVector;
private:
    string directory;
    void loadModel(const string &path);
    void processNode(aiNode* node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh);

//    static Assimp::Importer importer;
};
