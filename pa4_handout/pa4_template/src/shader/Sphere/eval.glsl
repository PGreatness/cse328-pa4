#version 410 core
layout (quads, equal_spacing, cw) in;

out vec3 Normal;
out vec3 fragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float R;
uniform float X;
uniform float Y;
uniform float Z;

const float PI = 3.14159265359;

void main()
{
    vec4 WC = gl_in[0].gl_Position;

    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    float phi = 2 * PI * u;
    float theta = PI * v;

    vec3 pos = vec3(R * sin(theta) * cos(phi) + X, R * sin(theta) * sin(phi) + Y, R * cos(theta) + Z);
    gl_Position = projection * view * model * vec4(pos, 1);

    fragPos = vec3(model * vec4(pos, 1));
    Normal = vec3(transpose(inverse(model)) * vec4(pos, 1));
}