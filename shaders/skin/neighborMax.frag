#version 410 core
layout (location = 0) out vec2 neighborMax;

in vec2 TexCoords;
// ---------------------------------------
uniform sampler2D tileMax;

void main()
{
    vec2 TexCoord = TexCoords;
    vec2 maxVelocity = vec2(0.0f);
    float largestMagnitude2 = 0.0f;
    for (int i = -3 / 2; i <= 3 / 2; i++) {
        for (int j = -3 / 2; j <= 3 / 2; j++) {
            vec2 index = TexCoord + vec2(i, j) / textureSize(tileMax, 0);
            vec2 v_G = texture(tileMax, index).xy;
            // Velocity at this point/tile
            float thisMagnitude2 = dot(v_G, v_G);

            if (thisMagnitude2 > largestMagnitude2) {
                // This is the new largest PSF
                maxVelocity = v_G;
                largestMagnitude2 = thisMagnitude2;
            }
        }
    }
    neighborMax.xy = maxVelocity;

    // neighborMax.xy = texture(tileMax, TexCoords).xy;
}