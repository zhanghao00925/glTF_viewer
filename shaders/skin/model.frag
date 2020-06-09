#version 410 core
// ouptut
out vec4 FragColor;
// vertex shader input
in vec2 TexCoords;
in vec3 WorldPos;
in vec3 Normal;
// Other
uniform vec3 camPos;
// Work Flow
uniform int work_flow;
// alpha
uniform int alpha_mode;
uniform float alpha_cutoff;
uniform int flag;
// emissive
uniform vec3 emissive_factor;
uniform sampler2D emissive_texture;
// normal
uniform float normal_scale;
uniform sampler2D normal_texture;
// occlusion
uniform float occlusion_strength;
uniform sampler2D occlusion_texture;
// metallic workflow
uniform vec4 baseColor_factor;
uniform sampler2D baseColor_texture;
uniform float metallic_factor;
uniform float roughness_factor;
uniform sampler2D metallicRoughness_texture;
// KHR_materials_pbrSpecularGlossiness
uniform vec4 diffuse_factor;
uniform sampler2D diffuse_texture;
uniform vec3 specular_factor;
uniform float glossiness_factor;
uniform sampler2D specular_glossiness_texture;
// ----------------------------------------------------------------------------
const float PI = 3.14159265359;
// ----------------------------------------------------------------------------
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}
// ----------------------------------------------------------------------------
vec3 getNormalFromMap()
{
    vec3 tangentNormal = texture(normal_texture, TexCoords).xyz * 2.0 - 1.0;

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
// ----------------------------------------------------------------------------
void main()
{
    vec3 c_diff;
    float alpha;
    vec3 F0;
    float roughness;
    if (0 == work_flow) {
        // METALLIC_ROUGHNESS
        // baseColor
        vec4 baseColor = baseColor_factor * pow(texture(baseColor_texture, TexCoords), vec4(2.2));
        alpha = baseColor.a;

        // metallic and roughness
        vec2 metallicRoughness = texture(metallicRoughness_texture, TexCoords).rg;
        float metallic = metallic_factor * metallicRoughness.r;
        float roughness = roughness_factor * metallicRoughness.g;
        const vec3 dielectricSpecular = vec3(0.04, 0.04, 0.04);
        const vec3 black = vec3(0, 0, 0);
        c_diff = mix(baseColor.rgb * (1 - dielectricSpecular.r), black, metallic);
        F0 = mix(dielectricSpecular, baseColor.rgb, metallic);
        roughness = roughness;
    } else {
        // SPECULAR_GLOSSINESS
        // diffuseColor
        vec4 diffuseColor = diffuse_factor * pow(texture(diffuse_texture, TexCoords), vec4(2.2));
        alpha = diffuseColor.a;
        // metallic and roughness
        vec4 specular_glossiness = pow(texture(specular_glossiness_texture, TexCoords), vec4(2.2));
        vec3 specular = specular_factor * specular_glossiness.rgb;
        float glossiness = glossiness_factor * specular_glossiness.a;
        c_diff = diffuseColor.rgb * (1 - max(specular.r, max(specular.g, specular.b)));
        F0 = specular;
        roughness = 1.0f - glossiness;
    }
    if (alpha_mode == 0) {
        alpha = 1.0;
    }
    if (alpha_mode == 1 && alpha < alpha_cutoff) {
        discard;
    }
    if (alpha_mode == 2 && alpha < 0.1) {
        discard;
    }
    // normal
    vec3 N;
    if ((flag & (1 << 1)) != 0) {
        N = getNormalFromMap();
    } else {
        N = Normal;
    }
    // View and reflect vector
    vec3 V = normalize(camPos - WorldPos);
    vec3 R = reflect(-V, N);
    // diffuse
    vec3 diffuse = c_diff / PI;
    // reflectance equation
    vec3 Lo = vec3(0.0);
    for(int i = 0; i < 1; ++i)
    {
        // calculate per-light radiance
        vec3 L = normalize(vec3(1000.0f) - WorldPos);
        vec3 H = normalize(V + L);

        float distance = length(vec3(1000.0f)  - WorldPos) / 100.0f;
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = vec3(1000.0f) * attenuation;

        // Cook-Torrance BRDF
        float NDF = DistributionGGX(N, H, roughness);
        float G   = GeometrySmith(N, V, L, roughness);
        vec3 F    = fresnelSchlickRoughness(max(dot(H, V), 0.0), F0, roughness);

        vec3 nominator    = NDF * G * F;
        float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; // 0.001 to prevent divide by zero.
        vec3 specular = nominator / denominator;

        // scale light by NdotL
        float NdotL = max(dot(N, L), 0.0);

        // add to outgoing radiance Lo
        Lo += ((1-F) * diffuse + specular) * radiance * NdotL; // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
    }
    vec3 color = Lo;
    // occlusion
    if ((flag & (1 << 2)) != 0) {
        vec3 occlusion = texture(occlusion_texture, TexCoords).rgb;
        color = mix(color, color * occlusion, occlusion_strength);
    }
    // emissive
    if ((flag & (1 << 0)) != 0) {
        vec3 emissive = emissive_factor * texture(emissive_texture, TexCoords).rgb;
        color += emissive;
    }
    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correct
    color = pow(color, vec3(1.0/2.2));

    FragColor = vec4(color, alpha);
}