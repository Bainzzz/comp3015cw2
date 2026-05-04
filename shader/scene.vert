#version 460

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexTexCoord;

out vec3 FragPos;        
out vec3 Normal;           
out vec2 TexCoord;        
out vec4 FragPosLightSpace; 

uniform mat4 ModelViewMatrix;
uniform mat4 ModelMatrix;       
uniform mat4 MVP;
uniform mat3 NormalMatrix;
uniform mat4 LightSpaceMatrix;  

void main()
{
    FragPos  = vec3(ModelViewMatrix * vec4(VertexPosition, 1.0));
    Normal   = normalize(NormalMatrix * VertexNormal);
    TexCoord = VertexTexCoord;

    FragPosLightSpace = LightSpaceMatrix * ModelMatrix * vec4(VertexPosition, 1.0);

    gl_Position = MVP * vec4(VertexPosition, 1.0);
}