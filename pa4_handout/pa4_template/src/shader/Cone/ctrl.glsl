#version 410 core

layout (vertices = 3) out;

out vec3 normal[];
out vec3 position[];

uniform vec3 center;
uniform float radius;
uniform float height;

const float PI = 3.14159265359;

void main() {
    // Calculate the positions of the vertices for the base of the cone
    vec3 v0 = vec3(center.x, center.y, center.z);
    for (int i = 0; i < 2; i++) {
        float angle = 2.0 * PI * (float(i) / 2.0);
        vec3 v = vec3(center.x + radius * cos(angle), center.y, center.z + radius * sin(angle));
        position[i] = v;
    }

    // Calculate the position of the vertex for the top of the cone
    position[2] = vec3(center.x, center.y + height, center.z);

    // Calculate the normal vector for each vertex
    for (int i = 0; i < 3; i++) {
        normal[i] = normalize(cross(position[(i + 1) % 3] - position[i], position[(i + 2) % 3] - position[i]));
    }

    // Pass the positions and normals to the tessellation evaluation shader
    gl_TessLevelOuter[0] = 3;
    gl_TessLevelInner[0] = 1;

    for(int i = 0; i < 3; i++){
        gl_out[i].gl_Position = gl_in[i].gl_Position;
    }
}
