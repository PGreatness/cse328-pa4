#version 410 core
layout (vertices = 1) out;

out vec3 position[];

uniform float centerZ;
uniform float radius;
uniform float centerY = 0.0; // Static Y value

const float PI = 3.1415926535897932384626433832795;
const int NUM_SEGMENTS = 100;

void main()
{
    // Calculate the positions of the vertices for the circle
    vec3 v0 = vec3(0.0, centerY, centerZ); // Center position
    for (int i = 0; i < NUM_SEGMENTS; i++)
    {
        float angle = 2.0 * PI * (float(i) / float(NUM_SEGMENTS));
        vec3 v = vec3(radius * cos(angle), centerY, centerZ + radius * sin(angle));
        position[i] = v;
    }

    // Pass the positions to the tesselation evaluation shader
    gl_TessLevelOuter[0] = NUM_SEGMENTS + 1;
    gl_TessLevelOuter[1] = 1;
    gl_TessLevelInner[0] = NUM_SEGMENTS + 1;
    gl_TessLevelInner[1] = 1;

    // Output the position of the control point
    position[NUM_SEGMENTS] = v0;
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}
