#version 440 core
out vec4 FragColor;
in vec2 TexCoords;
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
#define MAX_FRAGMENTS 75
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
uniform sampler2DMS colorTextureMS;
const int MAX_SAMPLES = 4;
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
    
    NodeType frags[MAX_FRAGMENTS];
    int count = 0;
    // Get the index of the head of the list
    uint index = imageLoad(headPointers, ivec2(gl_FragCoord.xy)).r;

    // Copy the linked list for this fragment into an array
    while(index != 0xffffffff && count < MAX_FRAGMENTS) {
        frags[count] = nodes[index];
        index = frags[count].next;
        count++;
    }
    //merge sort
    int i, j1, j2, k;
    int a, b, c;
    int step = 1;
    NodeType leftArray[MAX_FRAGMENTS/2]; //for merge sort
    while (step <= count)
    {
        i = 0;
        while (i < count - step)
        {
            ////////////////////////////////////////////////////////////////////////
            //merge(step, i, i + step, min(i + step + step, count));
            a = i;
            b = i + step;
            c = (i + step + step) >= count ? count : (i + step + step);

            for (k = 0; k < step; k++)
                leftArray[k] = frags[a + k];
            
            j1 = 0;
            j2 = 0;
            for (k = a; k < c; k++)
            {
                if (b + j1 >= c || (j2 < step && leftArray[j2].depth > frags[b + j1].depth))
                    frags[k] = leftArray[j2++];
                else
                    frags[k] = frags[b + j1++];
            }
            ////////////////////////////////////////////////////////////////////////
            i += 2 * step;
        }
        step *= 2;
    } 

    // Traverse the array, and combine the colors using the alpha channel.
    for( int i = 0; i < count; i++ )
    {
        color = mix(color, frags[i].color, frags[i].color.a);
        // color.rgb += frags[i].color.rgb * frags[i].color.a;
    }

    // HDR tonemapping
    vec3 rgb = vec3(color);
    rgb = uncharted2_filmic(rgb);
    // gamma correct
    rgb = pow(rgb, vec3(1.0/2.2));

    FragColor = vec4(rgb, 1.0f);
}