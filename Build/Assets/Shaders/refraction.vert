#version 460 core
layout (location = 0) in vec3 a_position;
layout (location = 2) in vec3 a_normal;


out vec3 v_texcoord;
uniform float u_ior = 1.3;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
void main()
{
	//tranform position and normal to world space
	vec3 position=vec3(u_model*vec4(a_position,1));
	vec3 normal =normalize(mat3(u_model) * a_normal);
	
	vec3 viewPosition=inverse(u_view)[3].xyz;
	vec3 viewDirection=normalize(position-viewPosition);

	// refract view direction about vertex normal using index of refraction
	v_texcoord = refract(viewDirection, normal, 1.0 / u_ior);	

	gl_Position = u_projection* u_view*u_model* vec4(a_position, 1.0);
}
