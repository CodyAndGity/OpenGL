#version 460 core

#define MAX_LIGHTS 5

#define POINT_LIGHT			0
#define DIRECTIONAL_LIGHT	1
#define SPOT_LIGHT			2

#define BASE_MAP		(1<<0)
#define SPECULAR_MAP	(1<<1)
#define EMISSIVE_MAP	(1<<2)
#define NORMAL_MAP		(1<<3)
#define CUBE_MAP		(1<<4)
#define SHADOW_MAP		(1<<5)


in VS_OUT{
	vec2 texcoord;
	vec3 position;
	vec3 normal;
	vec4 shadowCoord;
	mat3 tbn;
} fs_in;


out vec4 f_color;


uniform vec3 u_ambient_light;


struct Light{
	int type;
	bool shadowCaster;
	vec3 position;
	vec3 direction;
	vec3 color;
	float intensity;
	float range;
	float innerCutoff;
	float outerCutoff;
};
uniform int u_numLights=1;
uniform Light u_lights[MAX_LIGHTS];

uniform struct Material{
	vec3 baseColor;	
	vec3 emissiveColor;

	float shininess;
	vec2 tiling;
	vec2 offset;
	uint parameters;
} u_material;

uniform sampler2D u_baseMap;
uniform sampler2D u_specularMap;
uniform sampler2D u_emissiveMap;
uniform sampler2D u_normalMap;
uniform sampler2D u_shadowMap;

vec3 calculateNormal()
{
	// generate the normals from the normal map
	vec3 normal = texture(u_normalMap, fs_in.texcoord).rgb;
	// convert rgb normal (0 <-> 1) to xyx (-1 <-> 1)
	normal = normalize((normal*2) -1);
	// transform normals to model view space
	normal = normalize(fs_in.tbn * normal);

	return normal;
}



float calculateAttenuation(in float light_distance,in float range){
	float attenuation=max(0,1-(light_distance/range));
 
 
	return attenuation*attenuation;
}

float calculateShadow(in vec4 shadowCoord,in float bias){
	return texture(u_shadowMap,shadowCoord.xy).z<shadowCoord.z-bias? 0.0:1.0;
}
vec3 calculateLight(in Light light,in vec3 position,in vec3 normal,in float specularMask){

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
			light_direction=-(light.direction);
		break;

		case SPOT_LIGHT:
			{
			light_direction=normalize( light.position-position);
			float light_distance=length(light.position -position);

			attenuation=calculateAttenuation(light_distance,light.range);

			float angle =acos(dot(light_direction,-light.direction));
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
	vec3 view_direction=normalize(-position);

	//blinn phong specular model
	vec3 halfway_direction=normalize(light_direction+view_direction);
	 intensity=max(dot(normal,halfway_direction),0);
	 intensity=pow(intensity,u_material.shininess);
	vec3 specular=vec3(intensity) * specularMask;

	 //phong
	//vec3 reflection=reflect(-light_direction,normal);
	//intensity =max( dot(reflection,view_direction),0);
	//intensity=pow(intensity,u_material.shininess);


	return (diffuse+specular)*light.intensity*attenuation;
}

void main()
{
	vec3 normal = ((u_material.parameters & NORMAL_MAP) != 0u)
	? calculateNormal()
	: fs_in.normal;
	//vec3 color=calculateLight(fs_in.position,fs_in.normal);
	//f_color = texture(u_material.baseMap, fs_in.texcoord)*vec4(color,1);	
	float specularMask=((u_material.parameters & SPECULAR_MAP) !=0u) 
		?texture(u_specularMap,fs_in.texcoord).r
		:1;
	vec3 color=u_ambient_light;
	for(int i=0;i<u_numLights;i++){
	float shadow=(u_lights[i].shadowCaster &&((u_material.parameters & SHADOW_MAP)!=0u))
		? calculateShadow(fs_in.shadowCoord,0.001)
		:1.0;

		color+=calculateLight(u_lights[i],fs_in.position,normal,specularMask) * shadow;
	}

	vec4 emissive=((u_material.parameters & EMISSIVE_MAP) !=0u) 
		?texture(u_emissiveMap,fs_in.texcoord)* vec4(u_material.emissiveColor,1):
		vec4(u_material.emissiveColor,1);
	

	f_color = texture(u_baseMap, fs_in.texcoord)*vec4(color,1)+emissive;	
	//f_color=vec4(gl_FragCoord.x,gl_FragCoord.y,0,1);
}