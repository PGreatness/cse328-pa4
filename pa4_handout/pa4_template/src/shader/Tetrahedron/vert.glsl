#version 410 core

layout (location = 0) in vec3 aPos;

out vec3 ourFragColor;
out vec3 fragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 tetraColor;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    Normal = vec3(transpose(inverse(model)) * vec4(aPos, 1.0));
    fragPos = vec3(model * vec4(aPos, 1.0));
    ourFragColor = tetraColor;
}
