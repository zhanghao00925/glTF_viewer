#version 410 core
layout (location = 0) out vec2 tileMax;

in vec2 TexCoords;
// ---------------------------------------
uniform sampler2D gPosition;

uniform int maxMotionBlurRadius;

void main()
{
    vec2 TexCoord = TexCoords;
    vec2 maxVelocity = vec2(0.0f);
    float largestMagnitude2 = 0.0f;
    for (int i = -maxMotionBlurRadius / 2; i <= maxMotionBlurRadius / 2; i++) {
        for (int j = -maxMotionBlurRadius / 2; j <= maxMotionBlurRadius / 2; j++) {
            vec2 index = TexCoord + vec2(i, j) / textureSize(gPosition, 0);
            vec2 v_G = texture(gPosition, index).xy;
            // Velocity at this point/tile
            float thisMagnitude2 = dot(v_G, v_G);

            if (thisMagnitude2 > largestMagnitude2) {
                // This is the new largest PSF
                maxVelocity = v_G;
                largestMagnitude2 = thisMagnitude2;
            }
        }
    }
    // tileMax.xy = texture(gPosition, TexCoords).xy;
    tileMax.xy = maxVelocity;
}