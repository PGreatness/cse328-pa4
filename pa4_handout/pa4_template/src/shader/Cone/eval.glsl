#version 410 core

layout(triangles, equal_spacing, cw) in;


out vec3 Normal;
out vec3 fragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 center;
uniform float radius;
uniform float height;

const float PI = 3.1415926535897932384626433832795;

void main() {
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    // this should end up making a cone
    vec3 pos = vec3(center.x + radius * cos(u * 2.0 * PI), center.y + radius * sin(u * 2.0 * PI), center.z + height * v);

    gl_Position = projection * view * model * vec4(pos, 1.0);

    Normal = normalize(pos - center);
    fragPos = pos;

}
