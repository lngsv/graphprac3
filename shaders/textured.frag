#version 330 core

in vec2 fragTexCoord;

out vec4 color;

uniform sampler2D tex;

void main()
{
    color = vec4(min(texture(tex, fragTexCoord).xyz, vec3(1, 1, 1)), 1);
}
