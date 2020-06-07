#version 410 core core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform sampler2D gAuxBuffer;
uniform sampler2D LUT;

vec4 GetDepth() {
    return vec4(vec3(texture(gPosition, TexCoords).z * 100.0f) , 1.0f);
}
vec4 GetPostion() {
    return vec4(texture(gPosition, TexCoords).xyz, 1.0f);
}

vec4 GetNormal() {
    return vec4(texture(gNormal, TexCoords));
}
vec4 GetColor() {
//    return vec4(1.0f);
    return vec4(texture(gAlbedoSpec, TexCoords).xyz, 1.0f);
}

vec4 GetInstance() {
    float index = (texture(gAuxBuffer, TexCoords)).r;
    return vec4(texture(LUT,vec2(index)));
}

vec4 GetMaterial() {
    float index = (texture(gAlbedoSpec, TexCoords)).a;
    return vec4(texture(LUT,vec2(index)));
}

void main()
{
    color = GetDepth();
}