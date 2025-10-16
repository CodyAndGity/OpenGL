#version 460 core

layout (location=0) in vec3 a_position;
layout (location=1) in vec3 a_color;
layout (location=2) in vec2 a_texcoord;

out vec3 v_color;
out vec2 v_texcoord;

uniform float u_time;
uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
void main(){
	//float frequency = 3.0;
	//float amplitude = 0.3;
	vec3 position = a_position;
	v_texcoord=a_texcoord;
	//float offset = sin(u_time * frequency + position.y) * amplitude;
	//position.x += offset;
	float x =u_time;
	gl_Position = u_projection*u_view*u_model*vec4(position, 1.0);
	v_color=a_color;
	//gl_Position =vec4(a_position *sin(u_time), 1.0f);
}