/***************************/
/*  FILE NAME: vertex.cpp  */
/***************************/
#include "vertex.h"

/* default constructor */
Vertex::Vertex()
    : position()
    , normal()
    , texcoord()
    , joint()
    , weight()
{ /* empty */ }

/* copy constructor */
Vertex::Vertex(const Vertex& other)
    : position(other.position)
    , normal(other.normal)
    , texcoord(other.texcoord)
    , joint(other.joint)
    , weight(other.weight)
{ /* empty */ }