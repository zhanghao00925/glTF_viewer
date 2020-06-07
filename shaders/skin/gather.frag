#version 410 core
out vec4 FragColor;
in vec2 TexCoords;
// ---------------------------------------
uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D neighborMax;
// ----------------------------------------------------------------------------
float cone(float dist, float r) {
    return clamp(1.0 - abs(dist) / r, 0.0, 1.0);
}
// A cone filter with maximum weight 1 at dist = 0 and min weight 0 at |v|=dist.
float cylinder(float dist, float r) {
    // return 1.0 - smoothstep(r * 0.95, r * 1.05, abs(dist));

    // Alternative: (marginally faster on GeForce, comparable quality)
    return sign(r - abs(dist)) * 0.5 + 0.5;

    // The following gives nearly identical results and may be faster on some hardware,
    // but is slower on GeForce
    //    return (abs(dist) <= r) ? 1.0 : 0.0;
}
/** 0 if depth_A << depth_B, 1 if depth_A >> z_depth, fades between when they are close */
float softDepthCompare(float depth_A, float depth_B) {
    // World space distance over which we are conservative about the classification
    // of "foreground" vs. "background".  Must be > 0.  
    // Increase if slanted surfaces aren't blurring enough.
    // Decrease if the background is bleeding into the foreground.
    // Fairly insensitive
    const float SOFT_DEPTH_EXTENT = 0.0001;

    return clamp(1.0 - (depth_B - depth_A) / SOFT_DEPTH_EXTENT, 0.0, 1.0);
}
const float NEAR = 1.0f;// Projection matrix's near plane distance
const float FAR = 10000.0f;// Projection matrix's far plane distance
float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0;// Back to NDC
    return (2.0 * NEAR * FAR) / (FAR + NEAR - z * (FAR - NEAR));
}
float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}
// ----------------------------------------------------------------------------
const float EPSILON = 0.5;

void main() {
    vec2 X = TexCoords;
    vec2 v_n = texture(neighborMax, X).xy ;
    vec3 C_X = texture(gNormal, X).xyz;

    float v_n_length = max(EPSILON, length(v_n));
    if (v_n_length < 0.505) {
        FragColor = vec4(C_X, 1.0f);
        // FragColor = vec4(1.0, 1.0, 1.0, 1.0f);
        return;
    }
    vec2 V_X = texture(gPosition, X).xy;
    float V_X_length = max(EPSILON, length(V_X));

    float weight = 1.0 / V_X_length;
    vec3 sum = C_X * weight;

    const int S = 30;
    float j = clamp(rand(gl_FragCoord.xy + vec2(gl_SampleID) / 1000.0), 0.0, 1.0) - 0.5;
    for (int i = 0; i < S; i++) {
        if (i != (S + 1) / 2) {
            float t = mix(-1.0, 1.0, float(i + j + 1) / (S + 1.0));

            vec2 offset = t * v_n;
            float dist = length(offset);
            vec2 Y = X + offset / vec2(1920.0, 1080.0);
            vec2 V_Y = texture(gPosition, Y).xy;
            float V_Y_length = max(EPSILON, length(V_Y));

            float Z_X = LinearizeDepth(texture(gNormal, X).w);
            float Z_Y = LinearizeDepth(texture(gNormal, Y).w);
            float f = softDepthCompare(Z_X, Z_Y);
            float b = softDepthCompare(Z_Y, Z_X);

            float alpha_Y = f * cone(dist, V_Y_length) + b * cone(dist, V_X_length) + cylinder(dist, V_Y_length) * cylinder(dist, V_X_length) * 2.0f;
            vec3 C_Y = texture(gNormal, Y ).xyz;
            weight += alpha_Y;
            sum += alpha_Y * C_Y;
        }
    }

    FragColor = vec4((1.0f / weight) * sum, 1.0f);
    // FragColor = vec4(sum, 1.0f);

}