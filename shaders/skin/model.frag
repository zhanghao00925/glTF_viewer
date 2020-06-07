#version 440 core

in vec2 TexCoords;
in vec3 Normal;                                                                   
in vec3 WorldPos;

struct VSOutput
{
    vec2 TexCoord;
    vec3 Normal;                                                                   
    vec3 WorldPos;                                                                 
};

out vec4 FragColor;

void main()
{                                    
    VSOutput In;
    In.TexCoord = TexCoords;
    In.Normal   = normalize(Normal);
    In.WorldPos = WorldPos;                                                                  
                                                                                            
    FragColor = vec4(0.5f * (In.Normal + vec3(1.0)), 1);     
    // FragColor = vec4(1.0f);     
}
