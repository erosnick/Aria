#version 450 core

layout(location = 0) in vec4 offset;
layout(location = 1) in vec4 color;

out vec4 outColor;

const vec4 positions[3] = vec4[3](vec4(-0.5, -0.5, 0.0, 1.0), 
                                  vec4( 0.5, -0.5, 0.0, 1.0), 
							      vec4( 0.0,  0.5, 0.0, 1.0));

void main()
{
	gl_Position = positions[gl_VertexID] + offset;
	outColor = color;
}