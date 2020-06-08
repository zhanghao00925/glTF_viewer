/***************************/
/*  FILE NAME: vertex.cpp  */
/***************************/
#include "vertex.h"

/* default constructor */
GLTFVertex::GLTFVertex()
    : position()
    , normal()
    , texcoord()
    , joint()
    , weight()
{ /* empty */ }

/* copy constructor */
GLTFVertex::GLTFVertex(const GLTFVertex& other)
    : position(other.position)
    , normal(other.normal)
    , texcoord(other.texcoord)
    , joint(other.joint)
    , weight(other.weight)
{ /* empty */ }