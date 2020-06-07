/***********************/
/*  FILE NAME: skin.h  */
/***********************/
#ifndef _SKIN_H_
#define _SKIN_H_

#include "render_core.h"
/**************/
/*  INCLUDES  */
/**************/
#include <string>
#include <vector>

/**********************/
/*  CLASS NAME: Skin  */
/**********************/
class Skin
{
public:
    Skin();
    Skin(const Skin& other);

public:
    std::string name;
    std::vector<int> joints;
    std::vector<mat4> inverse_bind_matrices;
    int skeleton_root_id;
}; // class Skin 
#endif // !_SKIN_H_