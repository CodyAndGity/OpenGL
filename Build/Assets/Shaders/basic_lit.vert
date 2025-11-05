#version 460 core

#define MAX_LIGHTS 5

#define POINT_LIGHT			0
#define DIRECTIONAL_LIGHT	1
#define SPOT_LIGHT			2

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec2 a_texcoord;
layout (location = 2) in vec3 a_normal;

out VS_OUT{
	vec2 texcoord;
	vec3 color;
} vs_out;


uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

uniform vec3 u_ambient_light;

float calculateAttenuation(in float light_distance,in float range){
	float attenuation=max(0,1-(light_distance/range));
	//attenuation=clamp(attenuation,0,1);
 
 
 return attenuation*attenuation;
}

struct Light{
	int type;
	vec3 position;
	vec3 direction;
	vec3 color;
	float intensity;
	float range;
	float innerCutoff;
	float outerCutoff;
};

uniform struct Material{
	sampler2D baseMap;
	vec3 baseColor;

	float shininess;
	vec2 tiling;
	vec2 offset;
} u_material;
uniform int u_numLights=1;
uniform Light u_lights[MAX_LIGHTS];

vec3 calculateLight(in Light light,in vec3 position,in vec3 normal){

	vec3 light_direction;
	float attenuation=1.0f;

	switch(light.type){
		case POINT_LIGHT:
			{
				light_direction=normalize( light.position-position);
				float light_distance=length(light.position -position);
				attenuation=calculateAttenuation(light_distance,light.range);
			}
		break;

		case DIRECTIONAL_LIGHT:
			light_direction=(light.direction);
		break;

		case SPOT_LIGHT:
			{
			light_direction=normalize( light.position-position);
			float light_distance=length(light.position -position);

			attenuation=calculateAttenuation(light_distance,light.range);

			float angle =acos(dot(light_direction,light.direction));
			if(angle>light.outerCutoff){
				attenuation=0;
			}else{
				float spotAttenuation=smoothstep(light.outerCutoff+0.001,light.innerCutoff,angle);
				attenuation*=spotAttenuation;
				light.outerCutoff=max(light.outerCutoff,light.innerCutoff);
			}
			}
		break;

	}
	//diffuse
	float intensity =max( dot(light_direction,normal),0);
	vec3 diffuse=light.color*u_material.baseColor*intensity;

	//specular
	vec3 reflection=reflect(-light_direction,normal);
	vec3 view_direction=normalize(-position);
	intensity =max( dot(reflection,view_direction),0);
	intensity=pow(intensity,u_material.shininess);
	vec3 specular=vec3(intensity);


	return (diffuse+specular)*light.intensity*attenuation;
}
void main()
{
	
	vs_out.texcoord = a_texcoord*u_material.tiling+u_material.offset;

	mat4 model_view=u_view* u_model;
	vec3 position=vec3(model_view*vec4(a_position,1));
	vec3 normal=normalize(mat3(model_view)*a_normal);

	vs_out.color=u_ambient_light;
	for(int i=0;i<u_numLights;i++){
		vs_out.color+=calculateLight(u_lights[i],position,normal);
	}

	gl_Position = u_projection * u_view * u_model * vec4(a_position, 1.0);
}
