/*************************/
/*  FILE NAME: skin.cpp  */
/*************************/
#include "skin.h"

/* default constructor */
Skin::Skin()
    : name()
    , joints()
    , inverse_bind_matrices()
    , skeleton_root_id()
{
    skeleton_root_id = -1;
}

/* copy constructor */
Skin::Skin(const Skin& other)
    : name(other.name)
    , joints(other.joints)
    , inverse_bind_matrices(other.inverse_bind_matrices)
    , skeleton_root_id(other.skeleton_root_id)
{ /* empty */ }