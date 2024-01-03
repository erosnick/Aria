#version 450 core

const vec4 positions[3] = vec4[3](vec4(-0.5, -0.5, 0.0, 1.0), 
                                  vec4( 0.5, -0.5, 0.0, 1.0), 
							      vec4( 0.0,  0.5, 0.0, 1.0));

void main()
{
	gl_Position = positions[gl_VertexID];
}