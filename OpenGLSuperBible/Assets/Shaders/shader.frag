#version 450 core

in VSOut
{
	vec4 outColor;
} fsIn;

out vec4 fragColor;

void main()
{
    fragColor = fsIn.outColor;
}