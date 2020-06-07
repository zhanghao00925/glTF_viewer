#version 410 core core
layout (location = 0) out vec4 gNormal;
layout (location = 1) out vec4 gPosition;
layout (location = 2) out vec4 gAlbedoSpec;
layout (location = 3) out uvec4 gAuxBuffer;

in vec2 TexCoords;
// ---------------------------------------
uniform samplerCube gNormalCube;
uniform samplerCube gPositionCube;
uniform samplerCube gAlbedoSpecCube;
uniform usamplerCube gAuxBufferCube;

void main()
{	
    vec2 Coord = (TexCoords - 0.5f) * 2.0f;
    vec2 a = Coord * vec2(3.14159265, 1.57079633);
    // Convert to cartesian coordinates
    vec2 c = cos(a), s = sin(a);
    gNormal = texture(gNormalCube, vec3(vec2(s.x, c.x) * c.y, s.y));
    gPosition = texture(gPositionCube, vec3(vec2(s.x, c.x) * c.y, s.y));
    gAlbedoSpec = texture(gAlbedoSpecCube, vec3(vec2(s.x, c.x) * c.y, s.y));
    gAuxBuffer = texture(gAuxBufferCube, vec3(vec2(s.x, c.x) * c.y, s.y));
}