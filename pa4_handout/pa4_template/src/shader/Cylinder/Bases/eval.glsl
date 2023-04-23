#version 410 core
layout(triangles, equal_spacing, cw) in;

in vec3 Normal[];
in vec3 fragPos[];

out vec3 outColor;

void main()
{
    vec3 center = (fragPos[0] + fragPos[1] + fragPos[2]) / 3.0;

    vec3 v0 = fragPos[0] - center;
    vec3 v1 = fragPos[1] - center;
    vec3 v2 = fragPos[2] - center;

    vec3 normal = cross(v1 - v0, v2 - v0);
    normal = normalize(normal);

    vec3 pos = vec3(fragPos[0].x, center.y, fragPos[0].z);
    vec3 toCenter = center - pos;

    float radius = length(toCenter);

    outColor = vec3(1.0);

    gl_Position = vec4(fragPos[0].x, center.y, fragPos[0].z, 1.0);
}
