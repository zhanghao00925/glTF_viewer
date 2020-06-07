#include "model.h"

//Assimp::Importer Model::importer;

Model::Model(const string &path) {
    loadModel(path);
}

void Model::Draw(Shader &shader, mat4 &model, mat4 &view, mat4 &projection) {
    for (auto &mesh : meshMap) {
        mesh.second.Draw(shader, model, view, projection);
    }
}

void Model::loadModel(const string &path) {
    // Get scene
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);
    if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        cout << "ERROR:MODEL::LOADMODEL: "<< importer.GetErrorString();
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));
    processNode(scene->mRootNode, scene);
//    importer.FreeScene();
}

void Model::processNode(aiNode *node, const aiScene *scene) {
    aiMesh *mesh = nullptr;
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        mesh = scene->mMeshes[node->mMeshes[i]];
        string meshName((mesh->mName).C_Str());
        auto tempMesh = processMesh(mesh);
        meshMap.insert(make_pair(meshName,tempMesh));
        meshVector.push_back(tempMesh);
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh *mesh) {
    // Process mesh vertices
    vec3 position, normal;
    vec2 texcoords;
    vector<Vertex> vertices;
    vertices.reserve(mesh->mNumVertices);
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        // Set texture
        position.x = mesh->mVertices[i].x;
        position.y = mesh->mVertices[i].y;
        position.z = mesh->mVertices[i].z;
        // Set normal
        normal.x = mesh->mNormals[i].x;
        normal.y = mesh->mNormals[i].y;
        normal.z = mesh->mNormals[i].z;
        // Set texcoords
        if (mesh->mTextureCoords[0]) {
            texcoords.x = mesh->mTextureCoords[0][i].x;
            texcoords.y = mesh->mTextureCoords[0][i].y;
        } else {
            texcoords = vec2(0, 0);
        }
        vertices.emplace_back(position, normal, texcoords);
    }
    aiFace face;
    vector<GLuint> indices;
    indices.reserve(mesh->mNumFaces * 3);
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        face = mesh->mFaces[i];
        // push all triangle
        for (unsigned j = 0; j < face.mNumIndices; j++) {
            indices.emplace_back(face.mIndices[j]);
        }
    }
    return Mesh(vertices, indices);
}
