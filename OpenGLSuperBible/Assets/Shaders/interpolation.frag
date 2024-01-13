#version 450 core

in VSOut
{
	vec4 color;
} fsIn;

out vec4 fragColor;

void main()
{
    fragColor = fsIn.color;
}