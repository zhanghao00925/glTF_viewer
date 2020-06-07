#include "texture.h"

Texture::Texture(int channel) {
    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_2D, texId);
    unsigned char image[4] = {255, 255, 255, 255};
    GLenum format = GL_RGB;
    if (channel == 1)
        format = GL_RED;
    else if (channel == 3)
        format = GL_RGB;
    else if (channel == 4)
        format = GL_RGBA;
    glTexImage2D(GL_TEXTURE_2D, 0, format, 1, 1, 0, format, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture(const string &Filename) {
    int width, height, channel;
    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_2D, texId);
    // Read image
    if (stbi_is_hdr(Filename.c_str())) {
        float *image = nullptr;
        image = stbi_loadf(Filename.c_str(), &width, &height, &channel, 0);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, image);
        glGenerateMipmap(GL_TEXTURE_2D);
        // Free data
        stbi_image_free(image);
    } else {
        unsigned char *image = nullptr;
        image = stbi_load(Filename.c_str(), &width, &height, &channel, 0);
        GLenum format = GL_RGB;
        if (channel == 1)
            format = GL_RED;
        else if (channel == 3)
            format = GL_RGB;
        else if (channel == 4)
            format = GL_RGBA;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
        // Free data
        stbi_image_free(image);
    }
    // Set texture attrib
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture(const vector<string> &texturesPath) {
    int width, height, channel;
    unsigned char *image;
    // Create texture
    glGenTextures(1, &texId);
    // Bind texture
    glBindTexture(GL_TEXTURE_CUBE_MAP, texId);
    for (GLuint i = 0; i < texturesPath.size(); i++) {
        image = stbi_load(texturesPath[i].c_str(), &width, &height, &channel, 0);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        stbi_image_free(image);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}
