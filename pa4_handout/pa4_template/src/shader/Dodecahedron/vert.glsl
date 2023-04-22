#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 ourFragColor;
out vec3 fragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 dodecaColor;
uniform int isFlat = 0;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    if (isFlat == 1)
    {
        Normal = mat3(transpose(inverse(model))) * aNormal;
    } else {
        Normal = mat3(transpose(inverse(view))) * aPos;
    }
    fragPos = vec3(model * vec4(aPos, 1.0));
    ourFragColor = dodecaColor;
}
