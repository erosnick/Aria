#version 450 core

layout (triangles) in;
layout (points, max_vertices = 3) out;

in VSOut
{
    vec4 color;
} geoIn[];

out VSOut
{
    vec4 color;
} fsIn;

void main()
{
    int i;

    for (int i = 0; i < gl_in.length(); i++)
    {
        gl_Position = gl_in[i].gl_Position;
        fsIn.color = geoIn[i].color;
        EmitVertex();
    }
}