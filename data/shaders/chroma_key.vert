#version 330 core

layout (location = 0) in vec3 position;

const mat3 pos_mat = mat3(   0.5,  0.0,  0.5,
							 0.0, -0.5,  0.5,
							 0.0,  0.0,  0.0);
varying vec2 cur_pt; 
void main()
{
	cur_pt = (vec3(position.xy, 1.0)*pos_mat).xy;
	gl_Position = vec4(position,1.0);
}