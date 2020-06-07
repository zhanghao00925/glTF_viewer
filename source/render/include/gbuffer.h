#pragma once

#include "shader.h"

using namespace glm;

class GBuffer {
public:
    GBuffer();
    void Bind();
    void Unbind();
    void Release();
    void Draw(Shader &shader);
    void CopyDepthBuffer(GLuint destination, int width, int height);
    GLuint GetRenderTargetByIndex(int index);

    GLuint gPosition, gNormal, gAlbedoSpec, gAuxBuffer, gDepth;
private:
    GLuint gBuffer;
};
