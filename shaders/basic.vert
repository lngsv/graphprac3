#version 330 core

layout (location = 0) in vec3 position;

uniform mat4 transformMat;

void main()
{
    gl_Position = transformMat * vec4(position, 1.0);
}
