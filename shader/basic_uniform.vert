#version 460

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 ModelViewMatrix;
uniform mat4 MVP;
uniform mat3 NormalMatrix;

void main()
{
    Normal = normalize(NormalMatrix * VertexNormal);
    FragPos = (ModelViewMatrix * vec4(VertexPosition, 1.0)).xyz;
    gl_Position = MVP * vec4(VertexPosition, 1.0);
}