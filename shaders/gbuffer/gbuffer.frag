#version 410 core core
layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;
layout (location = 3) out uvec4 gAuxBuffer;

in vec2 TexCoords;
in vec3 WorldPos;
in vec3 Normal;

// Base Color
uniform vec3 baseColor;
uniform vec3 baseColorMapScale;
uniform sampler2D baseColorMap;
// metallic
uniform float metallic;
uniform sampler2D metallicMap;
// glossiness
uniform float glossiness;
uniform sampler2D glossinessMap;
// normal
uniform int bNormalMap;
uniform vec3 normalMapScale;
uniform sampler2D normalMap;
// semantic
uniform int catId;
uniform int instanceId;
// ---------------------------------------------------
const float NEAR = 1.0f;// Projection matrix's near plane distance
const float FAR = 10000.0f;// Projection matrix's far plane distance
float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0;// Back to NDC
    return (2.0 * NEAR * FAR) / (FAR + NEAR - z * (FAR - NEAR));
}
// ---------------------------------------------------
vec3 getNormalFromMap()
{
    vec3 tangentNormal = texture(normalMap, TexCoords * normalMapScale.xy).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(WorldPos);
    vec3 Q2  = dFdy(WorldPos);
    vec2 st1 = dFdx(TexCoords);
    vec2 st2 = dFdy(TexCoords);

    vec3 N   = normalize(Normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}
void main()
{
    // store position and metallic
    gPosition.xyz = WorldPos.xyz;
    gPosition.w = metallic * texture(metallicMap, TexCoords).r;
    // store normal and depth
    vec3 N;
    if (bNormalMap > 0.5) {
        N = getNormalFromMap();
    } else {
        N = normalize(Normal);
    }
    gNormal.xyz = clamp((N + vec3(1.0f)) / 2.0f, vec3(0.), vec3(1.));
    // gNormal.w = LinearizeDepth(gl_FragCoord.z);
    gNormal.w = gl_FragCoord.z;
    // store base color and glossiness
    gAlbedoSpec.xyz = baseColor * texture(baseColorMap, TexCoords * baseColorMapScale.xy).rgb;
    gAlbedoSpec.w = glossiness * texture(glossinessMap, TexCoords).r;
    // store semantic and instance
    gAuxBuffer.w = uint(catId);
    gAuxBuffer.x = uint((instanceId >> 16) & 0x00ff);
    gAuxBuffer.y = uint((instanceId >> 8 ) & 0x00ff);
    gAuxBuffer.z = uint((instanceId      ) & 0x00ff);
}