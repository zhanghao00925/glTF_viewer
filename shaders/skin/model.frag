#version 440 core

in vec2 TexCoords;
in vec3 Normal;                                                                   
in vec3 WorldPos;

out vec4 FragColor;

uniform sampler2D diffuse_texture;

void main()
{                                    
    vec3 normal  = normalize(Normal);
                            
    FragColor = vec4(texture(diffuse_texture, TexCoords).rgb, 1.0);                                                                
    // FragColor = vec4(0.5f * (In.Normal + vec3(1.0)), 1);     
    // FragColor = vec4(1.0f);     
}
