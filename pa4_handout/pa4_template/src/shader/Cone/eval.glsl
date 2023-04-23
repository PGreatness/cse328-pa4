#version 410 core

layout(triangles, equal_spacing, cw) in;

in vec3 position[];

out vec3 Normal;
out vec3 fragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    vec3 A = mix(position[0], position[1], u);
    vec3 B = mix(position[0], position[2], v);

    float h = length(B - A);
    vec3 P = mix(A, B, v / h);
    vec3 T = normalize(B - A);

    vec3 N = cross(T, vec3(0, 1, 0));
    if (length(N) < 0.01) {
        N = cross(T, vec3(1, 0, 0));
    }
    N = normalize(N);

    gl_Position = projection * view * model * vec4(P, 1);
    fragPos = vec3(model * vec4(P, 1));
    Normal = vec3(transpose(inverse(model)) * vec4(N, 0));
}
