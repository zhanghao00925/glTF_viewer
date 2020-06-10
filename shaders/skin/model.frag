#version 440 core
// ouptut
out vec4 FragColor;
// vertex shader input
in vec2 TexCoords;
in vec3 WorldPos;
in vec3 Normal;
// Other
uniform vec3 CameraPos;
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
vec3 sRGBToLinear(vec3 srgbIn)
{
    return vec3(pow(srgbIn.xyz, vec3(2.2)));
}
vec4 sRGBToLinear(vec4 srgbIn)
{
    return vec4(sRGBToLinear(srgbIn.xyz), srgbIn.w);
}
// ----------------------------------------------------------------------------
vec3 getNormalFromMap()
{
    vec3 tangentNormal = normalize(texture(normal_texture, TexCoords).xyz * 2.0 - 1.0);

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

// ----------------------------------------------------------------------------
void main()
{
    vec3 c_diff;
    float alpha;
    vec3 F0;
    float alphaRoughness;
    if (0 == work_flow) {
        // METALLIC_ROUGHNESS
        // baseColor
        vec4 baseColor = baseColor_factor * sRGBToLinear(texture(baseColor_texture, TexCoords));
        alpha = baseColor.a;
        // metallic and roughness
        float metallic = metallic_factor * texture(metallicRoughness_texture, TexCoords).b;
        float roughness = roughness_factor * texture(metallicRoughness_texture, TexCoords).g;
        const vec3 dielectricSpecular = vec3(0.04, 0.04, 0.04);
        const vec3 black = vec3(0, 0, 0);
        c_diff = mix(baseColor.rgb * (vec3(1.0f) - dielectricSpecular.r), black, metallic);
        F0 = mix(dielectricSpecular, baseColor.rgb, metallic);
        alphaRoughness = roughness;
    } else {
        // SPECULAR_GLOSSINESS
        // diffuseColor
        vec4 diffuseColor = diffuse_factor * sRGBToLinear(texture(diffuse_texture, TexCoords));
        alpha = diffuseColor.a;
        // metallic and roughness
        vec4 specular_glossiness = sRGBToLinear(texture(specular_glossiness_texture, TexCoords));
        vec3 specular = specular_factor * specular_glossiness.rgb;
        float glossiness = glossiness_factor * specular_glossiness.a;
        c_diff = diffuseColor.rgb * (1 - max(specular.r, max(specular.g, specular.b)));
        F0 = specular;
        alphaRoughness = 1.0f - glossiness;
    }
    if (alpha_mode == 0) {
        alpha = 1.0;
    }
    if (alpha_mode == 1) {
        if (alpha < alpha_cutoff) {
            discard;
        } else {
            alpha = 1.0;
        }
    }
    if (alpha_mode == 2 && alpha < 0.15) {
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
    vec3 V = normalize(CameraPos - WorldPos);
    vec3 R = reflect(-V, N);
    // diffuse
    vec3 diffuse = c_diff / PI;
    // reflectance equation
    vec3 Lo = vec3(0.0);
    vec3 lightPosition = vec3(500.0f);
    vec3 lightColor = vec3(10.0f);
    for(int i = 0; i < 1; ++i)
    {
        // calculate per-light radiance
        vec3 L = normalize(vec3(1000.0f) - WorldPos);
        vec3 H = normalize(V + L);

//        float distance = length(lightPosition  - WorldPos);
        //        float attenuation = 1.0 / (distance * distance);
        float attenuation = 1.0;
        // scale light by NdotL
        float NdotL = max(dot(N, L), 0.0);
        float NdotV = max(dot(N, V), 0.0);
        vec3 radiance = lightColor * attenuation;

        // Cook-Torrance BRDF
        float NDF = DistributionGGX(N, H, alphaRoughness);
        float G   = GeometrySmith(N, V, L, alphaRoughness);
        vec3 F    = fresnelSchlickRoughness(max(dot(H, V), 0.0), F0, alphaRoughness);

        vec3 nominator    = NDF * G * F;
        float denominator = max(4 * NdotV * NdotL, 0.001); // 0.001 to prevent divide by zero.
        vec3 specular = nominator / denominator;

        // add to outgoing radiance Lo
        Lo += ((1-F) * diffuse + specular) * radiance * NdotL; // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
//        Lo += (specular) * radiance * NdotL; // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
//        Lo += ((1-F) * diffuse) * radiance * NdotL; // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
    }
    vec3 color = Lo;
    // occlusion
    if ((flag & (1 << 2)) != 0) {
        float occlusion = texture(occlusion_texture, TexCoords).r;
        color = mix(color, color * occlusion, occlusion_strength);
    }
    // emissive
    if ((flag & (1 << 0)) != 0) {
        vec3 emissive = emissive_factor * sRGBToLinear(texture(emissive_texture, TexCoords).rgb);
        color += emissive;
    }
    // HDR tonemapping
//     color = color / (color + vec3(1.0));
    color = uncharted2_filmic(color);
    // gamma correct
    color = pow(color, vec3(1.0/2.2));

    FragColor = vec4(color, alpha);
//    FragColor = vec4(F0, alpha);
}