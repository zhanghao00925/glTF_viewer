#version 410 core
out vec4 FragColor;
in vec2 TexCoords;
// ----------------------------------------------------------------------------
uniform sampler2DMS colorTextureMS;
const int MAX_SAMPLES = 4;
// ----------------------------------------------------------------------------
vec3 uncharted2_tonemap_partial(vec3 x)
{
    float A = 0.15f;
    float B = 0.50f;
    float C = 0.10f;
    float D = 0.20f;
    float E = 0.02f;
    float F = 0.30f;
    return ((x*(A*x+C*B)+D*E)/(x*(A*x+B)+D*F))-E/F;
}
vec3 uncharted2_filmic(vec3 v)
{
    float exposure_bias = 2.0f;
    vec3 curr = uncharted2_tonemap_partial(v * exposure_bias);

    vec3 W = vec3(11.2f);
    vec3 white_scale = vec3(1.0f) / uncharted2_tonemap_partial(W);
    return curr * white_scale;
}
// ----------------------------------------------------------------------------
void main()
{
    // Average color
    vec4 color = vec4(0.0f);
    ivec2 iTexCoords = ivec2(textureSize(colorTextureMS) * TexCoords);
    float weight = 1.0f / MAX_SAMPLES;
    for (int i = 0; i < MAX_SAMPLES; i++) {
        vec4 colorSample = texelFetch(colorTextureMS, iTexCoords, i);
        color += weight * colorSample;
    }
    // HDR tonemapping
    vec3 rgb = vec3(color);
    rgb = uncharted2_filmic(rgb);
    // gamma correct
    rgb = pow(rgb, vec3(1.0/2.2));

    FragColor = vec4(rgb, 1.0f);
}