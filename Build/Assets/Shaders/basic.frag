#version 460 core

in vec3 v_color;
out vec4 fcolor;

uniform float u_time;

void main(){
	//fcolor=vec4(0,0,1,1);
	//float offest=sin(u_time)+gl_FragCoord.y*0.002;
	fcolor=vec4(v_color,1);
}