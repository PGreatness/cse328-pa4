#version 410 core
layout (triangles, equal_spacing, cw) in;

in vec3 position[];

out vec3 Normal;
out vec3 fragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec3 p0 = position[0];
    vec3 p1 = position[1];
    vec3 p2 = position[2];

    vec3 normal = normalize(cross(p1 - p0, p2 - p0));
    Normal = normal;

    vec3 pos = (p0 + p1 + p2) / 3.0;
    gl_Position = projection * view * model * vec4(pos, 1);

    fragPos = vec3(model * vec4(pos, 1));
}
