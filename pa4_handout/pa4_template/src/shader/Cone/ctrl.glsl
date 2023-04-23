#version 410

layout (vertices = 1) out;

void main()
{
    gl_TessLevelOuter[0] = 1080;
    gl_TessLevelOuter[1] = 1080;
    gl_TessLevelOuter[2] = 1080;
    gl_TessLevelOuter[3] = 1080;

    gl_TessLevelInner[0] = 64;
    gl_TessLevelInner[1] = 64;

    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}


