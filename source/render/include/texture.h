#pragma once

#include "render_core.h"

// stb library for image IO
#include "stb_image.h"
#include "stb_image_write.h"

class Texture {
public:
    Texture() {}
    void Release() {
        glDeleteTextures(1, &texId);
    }
    Texture(int channel);
    Texture(const string &Filename);
    Texture(const vector<string> &texturesPath);
    GLuint texId;
};
