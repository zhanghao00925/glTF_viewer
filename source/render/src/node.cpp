/*************************/
/*  FILE NAME: node.cpp  */
/*************************/
#include "node.h"

/**************/
/*  INCLUDES  */
/**************/

/* default constructor */
Node::Node()
    : name()
    , child_ids()
    , matrix()
    , translate()
    , rotate()
    , scale()
    , node_id()
    , parent_id()
    , mesh_id()
    , skin_id()
{
    matrix = glm::identity<mat4>();
    scale = vec3(1.0f);
    node_id = -1;
    parent_id = -1;
    mesh_id = -1;
    skin_id = -1;
}

/* copy constructor */
Node::Node(const Node& other)
    : name(other.name)
    , child_ids(other.child_ids)
    , matrix(other.matrix)
    , translate(other.translate)
    , rotate(other.rotate)
    , scale(other.scale)
    , node_id(other.node_id)
    , parent_id(other.parent_id)
    , mesh_id(other.mesh_id)
    , skin_id(other.skin_id)
{ /* empty */ }

/* function to return local translation of node */
mat4 Node::LocalMatrix() const
{
    mat4 scaleM = glm::scale(mat4(1.0f), scale);
    mat4 rotateM = glm::toMat4(rotate);
    mat4 translateM = glm::translate(mat4(1.0f), translate);
    // return matrix * scaleM * rotateM * translateM;
    return  translateM * rotateM  * scaleM * matrix;
}