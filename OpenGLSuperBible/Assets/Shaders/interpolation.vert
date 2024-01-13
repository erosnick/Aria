#version 450 core

layout(location = 0) in vec4 offset;

out VSOut
{
	vec4 color;
} vsOut;

const vec4 positions[3] = vec4[3](vec4(-0.5, -0.5, 0.0, 1.0), 
                                  vec4( 0.5, -0.5, 0.0, 1.0), 
							    vec4( 0.0,  0.5, 0.0, 1.0));

const vec4 colors[] = vec4[3](vec4(1.0, 0.0, 0.0, 1.0),
							  vec4(0.0, 1.0, 0.0, 1.0),
							  vec4(0.0, 0.0, 1.0, 1.0));

void main()
{
	gl_Position = positions[gl_VertexID] + offset;
	vsOut.color = colors[gl_VertexID];
}