/************************/
/*  FILE NAME: model.h  */
/************************/
#ifndef _MODEL_H_
#define _MODEL_H_

/**************/
/*  INCLUDES  */
/**************/
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <map>

#include "mesh.h"
#include "node.h"
#include "skin.h"
#include "animation.h"
#include "texture.h"
#include "material.h"
#include "tiny_gltf.h"

#include "render_core.h"
/***********************/
/*  CLASS NAME: Model  */
/***********************/
class Model
{
public:
    Model(const std::string& filename);
    Model(const Model& other);
    ~Model();

public:
    void SetupModel();
    void CleanupModel();
    void Update(double total_time);
    void RenderNode(const Shader& shader, int node_id);
    void RenderTransparent(const Shader &shader);
    void Render(const Shader& shader);

public:
    bool IsAnimated() const;
    void ChangeAnimation(int num);

private:
    void LoadModel(const std::string& file);
    mat4 GetNodeMatrix(int node_id);
    void UpdateAnimation(double duration);
    void UpdateNode(int node_id);

private:
    struct RenderDependence {
        int mesh_id;
        int skin_id;
        vector<float> weights;
    };
    struct TransparentCompare{
        bool operator()(pair<float, RenderDependence> x,pair<float, RenderDependence> y)
        {
            return x.first > y.first; // x is front
        }
    };
    std::priority_queue<pair<float, RenderDependence>, vector<pair<float, RenderDependence>>, TransparentCompare> transparent_queue;
    size_t curr_animation;
    std::string filename;
    std::map<int, Mesh> meshes;
    std::map<int, Node> nodes;
    std::map<int, Skin> skins;
    std::map<int, Animation> animations;
    std::map<int, Texture> textures;
    std::map<int, Material> materials;
}; // class Model
#endif // !_MODEL_H_