#version 410 core

layout(triangles, equal_spacing, ccw) in;

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

    vec3 pos;
    vec3 normal;

    if (u == 1.0 || u == 0.0) {
        // Top vertex
        pos = vec3(center.x, u == 1.0 ? center.y + height : center.y, center.z);
        normal = vec3(0.0, u == 1.0 ? 1.0 : -1.0, 0.0);
    } else {
        // Base vertices
        float theta = 2.0 * PI * v;
        float x = radius * cos(theta);
        float z = radius * sin(theta);
        pos = vec3(center.x + x, center.y, center.z + z);
        normal = normalize(vec3(x, height, z));
    }

    gl_Position = projection * view * model * vec4(pos, 1.0);
    fragPos = pos;
    Normal = normalize(mat3(transpose(inverse(model))) * normal);
}
