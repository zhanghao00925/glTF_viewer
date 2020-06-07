#version 410 core
out vec4 FragColor;
in vec2 TexCoords;
// ---------------------------------------
uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform sampler2D gAuxBuffer;
uniform sampler2D gDepthMap;
// IBL
uniform samplerCube irradianceMap;
uniform samplerCube prefilterMap;
uniform sampler2D brdfLUT;
// ---------------------------------------
// lights
// lights
const int MAX_LIGHT = 32;
uniform vec3 lightPositions[MAX_LIGHT];
uniform vec3 lightColors[MAX_LIGHT];
uniform int lightCount;
uniform vec3 camPos;
// ---------------------------------------
uniform mat4 view;
uniform mat4 projection;
// ---------------------------------------
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
vec3 WorldPosFromDepth(float depth) {
    float z = depth * 2.0 - 1.0;
    vec4 clipSpacePosition = vec4(vec3(gl_FragCoord.x / 1920.0 ,gl_FragCoord.y / 1080.0, depth) * 2.0 - 1.0, 1.0);
    vec4 viewSpacePosition =  inverse(projection) * clipSpacePosition;
    // Perspective division
    viewSpacePosition /= viewSpacePosition.w;
    vec4 worldSpacePosition = inverse(view) * viewSpacePosition;
    return worldSpacePosition.xyz;
}

// ----------------------------------------------------------------------------
void PBR() {
    // vec3 WorldPos = texture(gPosition, TexCoords).xyz;
    // vec3 WorldPos = WorldPosFromDepth(texture(gNormal, TexCoords).w);
    vec3 WorldPos = WorldPosFromDepth(texture(gDepthMap, TexCoords).x);
    vec3 albedo     = pow(texture(gAlbedoSpec, TexCoords).rgb, vec3(2.2));
    float metallic  = texture(gPosition, TexCoords).a;
    // float metallic  = 1.0f; // for test
    float roughness = 1.0f - texture(gAlbedoSpec, TexCoords).a;
    float ao        = 1.0f;

    vec3 N = texture(gNormal, TexCoords).xyz * 2.0f - vec3(1.0f);
    vec3 V = normalize(camPos - WorldPos);  
    vec3 R = reflect(-V, N);

    // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

    // reflectance equation
    vec3 Lo = vec3(0.0);
    for(int i = 0; i < lightCount; ++i)
    {
        // calculate per-light radiance
        vec3 L = normalize(lightPositions[i] - WorldPos);
        vec3 H = normalize(V + L);
        
        float distance = length(lightPositions[i] - WorldPos) / 10.0f; // dm
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = lightColors[i] * attenuation;

        // Cook-Torrance BRDF
        float NDF = DistributionGGX(N, H, roughness);
        float G   = GeometrySmith(N, V, L, roughness);
        vec3 F    = fresnelSchlickRoughness(max(dot(H, V), 0.0), F0, roughness);

        vec3 nominator    = NDF * G * F;
        float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001; // 0.001 to prevent divide by zero.
        vec3 specular = nominator / denominator;

        // kS is equal to Fresnel
        vec3 kS = F;
        // for energy conservation, the diffuse and specular light can't
        // be above 1.0 (unless the surface emits light); to preserve this
        // relationship the diffuse component (kD) should equal 1.0 - kS.
        vec3 kD = vec3(1.0) - kS;
        // multiply kD by the inverse metalness such that only non-metals
        // have diffuse lighting, or a linear blend if partly metal (pure metals
        // have no diffuse light).
        kD *= 1.0 - metallic;

        // scale light by NdotL
        float NdotL = max(dot(N, L), 0.0);

        // add to outgoing radiance Lo
        Lo += (kD * albedo / PI + specular) * radiance * NdotL; // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
    }

   // ambient lighting (we now use IBL as the ambient term)
   vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);

   vec3 kS = F;
   vec3 kD = 1.0 - kS;
   kD *= 1.0 - metallic;

   vec3 irradiance = texture(irradianceMap, N).rgb;
   vec3 diffuse    = irradiance * albedo;

   const float MAX_REFLECTION_LOD = 4.0;
   vec3 prefilteredColor = textureLod(prefilterMap, R,  roughness * MAX_REFLECTION_LOD).rgb;
   vec2 envBRDF  = texture(brdfLUT, vec2(max(dot(N, V), 0.0), roughness)).rg;
   vec3 specular = prefilteredColor * (F * envBRDF.x + envBRDF.y);

   vec3 ambient = (kD * diffuse + specular) * ao;
    // vec3 ambient = vec3(0.002);
    vec3 color = ambient + Lo;
    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correct
    color = pow(color, vec3(1.0/2.2));
    FragColor = vec4(color , 1.0);
}
void main()
{
    PBR();
//    FragColor = vec4((texture(gNormal, TexCoords).xyz + vec3(1.0)) / 2.0f, 1.0f);
}