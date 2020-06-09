/****************************/
/*  FILE NAME: texture.cpp  */
/****************************/
#include "texture.h"
/**************/
/*  INCLUDES  */
/**************/
#include <stdexcept>
#include "render_core.h"

/*********************/
/*  STATIC VARIABLE  */
/*********************/
static constexpr int Wrap_Mode[] = 
{
    0,     // UNKNOWN
    10497, // REPEAT
    33069, // CLAMP_TO_BORDER,
    33071, // CLAMP_TO_EDGE
    33648  // GL_MIRRORED_REPEAT
};

static constexpr int Filter_Mode[] = 
{
    0,    // UNKNOWN
    9728, // NEAREST
    9729, // LINEAR
    9984, // NEAREST_MIPMAP_NEAREST
    9985, // LINEAR_MIPMAP_NEAREST
    9986, // NEAREST_MIPMAP_LINEAR
    9987  // LINEAR_MIPMAP_LINEAR
};

/* default constructor */
Texture::Texture()
    : name()
    , image()
    , sampler()
    , tbo()
{ /* empty */ }

/* copy constructor */
Texture::Texture(const Texture& other)
    : name(other.name)
    , image(other.image)
    , sampler(other.sampler)
    , tbo(other.tbo)
{ /* empty */ }

/* function to make texture data available to OpenGL */
void Texture::SetupTexture()
{
    glGenTextures(1, &tbo);
    glBindTexture(GL_TEXTURE_2D, tbo);
    
    GLenum format;
    if(image.component == 1)
        format = GL_RED;
    else if(image.component == 3)
        format = GL_RGB;
    else if(image.component == 4)
        format = GL_RGBA;
    else throw std::runtime_error("Undefined Texture image format.");

    glTexImage2D(GL_TEXTURE_2D, 0, format, image.width, image.height, 0, format, GL_UNSIGNED_BYTE, image.data.data());
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, Wrap_Mode[static_cast<int>(sampler.wrap_R)]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, Wrap_Mode[static_cast<int>(sampler.wrap_S)]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, Wrap_Mode[static_cast<int>(sampler.wrap_T)]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Filter_Mode[static_cast<int>(sampler.min_filter)]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Filter_Mode[static_cast<int>(sampler.mag_filter)]);

    glBindTexture(GL_TEXTURE_2D, 0);
}

/* function to clean up texture data used in OpenGL */
void Texture::CleanupTexture()
{
    glDeleteTextures(1, &tbo);
}

/* function to bind OpenGL texture */
void Texture::BindTexture() const
{
    glBindTexture(GL_TEXTURE_2D, tbo);
}