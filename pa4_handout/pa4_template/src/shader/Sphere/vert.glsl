#version 410 core

layout (location = 0) in float null;
out vec3 ourFragColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 tetraColor;
uniform int isFlat = 0;

void main()
{
    gl_Position = vec4(0.0, 0.0, 0.0, 1.0);
    ourFragColor = tetraColor;
}
