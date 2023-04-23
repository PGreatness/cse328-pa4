#version 410 core
layout (quads, equal_spacing, cw) in;

out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float R;

const float PI = 3.14159265359;

void main()
{
    vec4 WC = gl_in[0].gl_Position;

    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    float phi = 2 * PI * u;
    float theta = PI * v;

    vec3 pos = vec3(R * cos(phi) * cos(theta), R * cos(phi) * sin(theta), R * sin(phi));
    gl_Position = projection * view * model * vec4(pos, 1);

    fragPos = vec3(model * vec4(pos, 1));
    Normal = vec3(transpose(inverse(model)) * vec4(pos, 1));
}