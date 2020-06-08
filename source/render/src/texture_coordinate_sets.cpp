/********************************************/
/*  FILE NAME: texture_coordinate_sets.cpp  */
/********************************************/
#include "texture_coordinate_sets.h"

/* default constructor */
TextureCoordinateSets::TextureCoordinateSets()
    : base_color()
    , metallic_roughness()
    , specular_glossiness()
    , normal()
    , occlusion()
    , emissive()
{
    base_color = 0;
    metallic_roughness = 0;
    specular_glossiness = 0;
    normal = 0;
    occlusion = 0;
    emissive = 0;
}

/* copy constructor */
TextureCoordinateSets::TextureCoordinateSets(const TextureCoordinateSets& other)
    : base_color(other.base_color)
    , metallic_roughness(other.metallic_roughness)
    , specular_glossiness(other.specular_glossiness)
    , normal(other.normal)
    , occlusion(other.occlusion)
    , emissive(other.emissive)
{ /* empty */ }