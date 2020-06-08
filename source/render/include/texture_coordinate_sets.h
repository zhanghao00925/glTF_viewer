/******************************************/
/*  FILE NAME: texture_coordinate_sets.h  */
/******************************************/
#ifndef _TEXTURE_COORDINATE_SETS_H_
#define _TEXTURE_COORDINATE_SETS_H_

/***************************************/
/*  CLASS NAME: TextureCoordinateSets  */
/***************************************/
class TextureCoordinateSets
{
public:
    TextureCoordinateSets();
    TextureCoordinateSets(const TextureCoordinateSets& other);

public:
    unsigned char base_color;
    unsigned char metallic_roughness;
    unsigned char specular_glossiness;
    unsigned char normal;
    unsigned char occlusion;
    unsigned char emissive;
}; // class TextureCoordinateSets
#endif // !_TEXTURE_COORDINATE_SETS_H_