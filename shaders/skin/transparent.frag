#version 440 core
layout(early_fragment_tests) in;
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
uniform int MAX_NODES;
struct NodeType {
  vec4 color;
  float depth;
  uint next;
};
layout( binding = 0, r32ui) uniform uimage2D headPointers;
layout( binding = 0, offset = 0) uniform atomic_uint nextNodeCounter;
layout( binding = 0, std430 ) buffer linkedLists {
    NodeType nodes[];
};
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
struct NormalInfo {
    vec3 ng;
    vec3 t;
    vec3 b;
    vec3 n;
};
NormalInfo getNormalFromMap(vec3 v)
{
    vec2 UV = TexCoords;
    vec3 uv_dx = dFdx(vec3(UV, 0.0));
    vec3 uv_dy = dFdy(vec3(UV, 0.0));

    vec3 t_ = (uv_dy.t * dFdx(WorldPos) - uv_dx.t * dFdy(WorldPos)) /
    (uv_dx.s * uv_dy.t - uv_dy.s * uv_dx.t);

    vec3 n, t, b, ng;
    // no geometrical tbn
    ng = normalize(Normal);
    t = normalize(t_ - ng * dot(ng, t_));
    b = cross(ng, t);
    float facing = step(0.0, dot(v, ng)) * 2.0 - 1.0;
    t *= facing;
    b *= facing;
    ng *= facing;
    vec3 direction;
    // no anisoptry
    direction = vec3(1.0, 0.0, 0.0);
    t = mat3(t, b, ng) * normalize(direction);
    b = normalize(cross(ng, t));

    if ((flag & (1 << 1)) != 0) {
        n = texture(normal_texture, UV).rgb * 2.0 - vec3(1.0);
        n *= vec3(normal_scale, normal_scale, 1.0);
        n = mat3(t, b, ng) * normalize(n);
    } else {
        n = ng;
    }

    NormalInfo info;
    info.ng = ng;
    info.t = t;
    info.b = b;
    info.n = n;
    return info;
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
vec4 PBR()
{
    vec3 c_diff;
    float alpha;
    vec3 F0;
    float alphaRoughness;
    if (0 == work_flow) {
        // METALLIC_ROUGHNESS
        // baseColor
        vec4 baseColor = baseColor_factor;
        if ((flag & (1 << 3)) != 0) {
            baseColor *= sRGBToLinear(texture(baseColor_texture, TexCoords));
        }
        alpha = baseColor.a;
        // metallic and roughness
        float metallic = metallic_factor;
        float roughness = roughness_factor;
        if ((flag & (1 << 4)) != 0) {
            metallic *= texture(metallicRoughness_texture, TexCoords).b;
            roughness *= texture(metallicRoughness_texture, TexCoords).g;
        }
        const vec3 dielectricSpecular = vec3(0.04, 0.04, 0.04);
        const vec3 black = vec3(0, 0, 0);
        c_diff = mix(baseColor.rgb * (vec3(1.0f) - dielectricSpecular.r), black, metallic);
        F0 = mix(dielectricSpecular, baseColor.rgb, metallic);
        alphaRoughness = roughness;
    } else if (1 == work_flow) {
        // SPECULAR_GLOSSINESS
        // diffuseColor
        vec4 diffuseColor = diffuse_factor;
        if ((flag & (1 << 5)) != 0) {
            diffuseColor *= sRGBToLinear(texture(diffuse_texture, TexCoords));
        }
        alpha = diffuseColor.a;
        // metallic and roughness
        vec4 specular_glossiness = sRGBToLinear(texture(specular_glossiness_texture, TexCoords));
        vec3 specular = specular_factor;
        float glossiness = glossiness_factor;
        if ((flag & (1 << 6)) != 0) {
            specular *= specular_glossiness.rgb;
            glossiness *= specular_glossiness.a;
        }
        c_diff = diffuseColor.rgb * (1 - max(specular.r, max(specular.g, specular.b)));
        F0 = specular;
        alphaRoughness = 1.0f - glossiness;
    } else {
        // unlit
        vec4 baseColor = baseColor_factor;
        if ((flag & (1 << 3)) != 0) {
            baseColor *= sRGBToLinear(texture(baseColor_texture, TexCoords));
        }
        return baseColor;
    }
    if (alpha_mode == 1) {
        if (alpha < alpha_cutoff) {
            discard;
        } else {
            alpha = 1.0;
        }
    }
    // View and reflect vector
    vec3 V = normalize(CameraPos - WorldPos);
    NormalInfo normalInfo = getNormalFromMap(V);
    vec3 N = normalInfo.n;
    vec3 R = reflect(-V, N);
    // diffuse
    vec3 diffuse = c_diff / PI;
    // reflectance equation
    vec3 Lo = vec3(0.0);
    vec3 lightPosition = vec3(500.0f);
    vec3 lightColor = vec3(0.5f);
    for(int i = 0; i < 1; ++i)
    {
        // calculate per-light radiance
        vec3 L = normalize(vec3(1000.0f) - WorldPos);
        vec3 H = normalize(V + L);

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
        // Lo += (specular) * radiance * NdotL; // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
        // Lo += ((1-F) * diffuse) * radiance * NdotL; // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
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
    return vec4(color, alpha);
}

void main()
{
  // Get the index of the next empty slot in the buffer
  uint nodeIdx = atomicCounterIncrement(nextNodeCounter);
  // Is our buffer full?  If so, we don't add the fragment
  // to the list.
  if( nodeIdx < MAX_NODES ) {

    // Our fragment will be the new head of the linked list, so
    // replace the value at gl_FragCoord.xy with our new node's
    // index.  We use imageAtomicExchange to make sure that this
    // is an atomic operation.  The return value is the old head
    // of the list (the previous value), which will become the
    // next element in the list once our node is inserted.
    uint prevHead = imageAtomicExchange(headPointers, ivec2(gl_FragCoord.xy), nodeIdx);

    // Here we set the color and depth of this new node to the color
    // and depth of the fragment.  The next pointer, points to the
    // previous head of the list.
    nodes[nodeIdx].color = PBR();
    nodes[nodeIdx].depth = gl_FragCoord.z;
    nodes[nodeIdx].next = prevHead;
  }
}
