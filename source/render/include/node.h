/***********************/
/*  FILE NAME: node.h  */
/***********************/
#ifndef _NODE_H_
#define _NODE_H_

/**************/
/*  INCLUDES  */
/**************/
#include <string>
#include <vector>

#include "render_core.h"
/**********************/
/*  CLASS NAME: Node  */
/**********************/
class Node
{
public:
    Node();
    Node(const Node& other);

public:
    mat4 LocalMatrix() const;

public:
    std::string name;
    std::vector<int> child_ids;

    mat4 matrix;
    vec3 translate;
    quat rotate;
    vec3 scale;
    std::vector<float> weights;

    int node_id;
    int parent_id;
    int mesh_id;
    int skin_id;
}; // class Node
#endif // !_NODE_H_