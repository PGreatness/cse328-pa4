#version 410 core

in vec3 ourFragColor;

out vec4 fragColor;

void main()
{
    fragColor = vec4(ourFragColor, 1);
}

