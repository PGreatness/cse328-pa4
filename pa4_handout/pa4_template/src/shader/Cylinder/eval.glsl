#version 410 core
layout (quads, equal_spacing, cw) in;

out vec3 Normal;
out vec3 fragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float radius;
uniform float height;
uniform vec3 xyz;

const float PI = 3.14159265359;

void main()
{
    vec4 WC = gl_in[0].gl_Position;

    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    vec3 pos;
    vec3 normal;

    if (u == 1.0 || u == 0.0) {
        // top or bottom face
        pos = vec3(radius * cos(2.0 * PI * v), height * (1.0 - u), radius * sin(2.0 * PI * v));
        normal = vec3(0.0, -1.0 + 2.0 * u, 0.0);
    } else {
        // curved surface
        pos = vec3(radius * cos(2.0 * PI * v), height * (u - 0.5), radius * sin(2.0 * PI * v));
        normal = vec3(pos.x, 0.0, pos.z);
    }

    
    gl_Position = projection * view * model * vec4(pos, 1);
    fragPos = vec3(model * vec4(pos, 1));
    Normal = vec3(transpose(inverse(model)) * vec4(normalize(normal), 0));
}
