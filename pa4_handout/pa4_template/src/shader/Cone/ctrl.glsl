#version 410

layout (vertices = 1) out;

void main()
{
    gl_TessLevelOuter[0] = 256;
    gl_TessLevelOuter[1] = 256;
    gl_TessLevelOuter[2] = 256;
    gl_TessLevelOuter[3] = 256;

    gl_TessLevelInner[0] = 64;
    gl_TessLevelInner[1] = 64;

    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}


