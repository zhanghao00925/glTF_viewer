/**************************/
/*  FILE NAME: texture.h  */
/**************************/
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

/**************/
/*  INCLUDES  */
/**************/
#include <string>
#include "image.h"
#include "texture_sampler.h"

/*************************/
/*  CLASS NAME: Texture  */
/*************************/
class Texture
{
public:
    Texture();
    Texture(const Texture& other);

public:
    void SetupTexture();
    void CleanupTexture();
    void BindTexture();

public:
    std::string name;
    Image image;
    TextureSampler sampler;

private:
    unsigned int tbo;
}; // class name Texture
#endif // !_TEXTURE_H_