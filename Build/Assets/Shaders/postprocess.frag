#version 430

#define GRAYSCALE		(1 << 0)
#define COLORTINT		(1 << 1)
#define SCANLINE		(1 << 2)
#define GRAIN			(1 << 3)
#define INVERT			(1 << 4)
#define EDGEDETECTION	(1 << 5)

in vec2 v_texcoord;

out vec4 f_color;

// post process controls
uniform uint u_parameters = 0;
uniform vec3 u_color_tint = vec3(0);
uniform float u_time = 0;
uniform float u_blend = 0;  // blend between original and postprocess color

uniform sampler2D u_baseMap;

// There is no random function in GLSL, uses math function to generate psuedo random number
// https://thebookofshaders.com/10/
float random(vec2 st) {
	return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

//EdgeDetection

uniform mat3 u_kernel;

vec4 edgeCalculation(){

	ivec2 texture_size = textureSize(u_baseMap, 0);
    vec2 step_size = 1.0/texture_size;

    vec2 offsets[9] = vec2[](
        vec2(-step_size.x,  -step_size.y),
        vec2( 0.0f,         -step_size.y),
        vec2( step_size.x,  -step_size.y),
        vec2(-step_size.x,   0.0f),
        vec2( 0.0f,          0.0f),
        vec2( step_size.x,   0.0f),
        vec2(-step_size.x,  +step_size.y),
        vec2( 0.0f,         +step_size.y),
        vec2( step_size.x,   step_size.y)
    );

    
    vec4 sum = vec4(0.0);
    int offset=0;
    for(int row = 0; row < 3; row++) {
        for(int column = 0; column < 3; column++) {
            sum += texture(u_baseMap, v_texcoord + offsets[offset]) * u_kernel[row][column];
            offset++;
        }
    }

    sum.a = 1.0;
    return sum;
}

void main()
{
	vec4 color = texture(u_baseMap, v_texcoord);
	vec4 postprocess = color;
	// apply post process events
	if ((u_parameters & EDGEDETECTION) != 0u)	postprocess = edgeCalculation();
	if ((u_parameters & GRAYSCALE) != 0u)		postprocess = vec4(vec3((postprocess.r+postprocess.g+postprocess.b)/3),1);
	if ((u_parameters & COLORTINT) != 0u)		postprocess = postprocess*vec4(u_color_tint,1);
	if ((u_parameters & SCANLINE) != 0u)		postprocess = (int(gl_FragCoord.y) % 3 != 0) ? vec4(0, 0, 0, 1) : postprocess;
	if ((u_parameters & GRAIN) != 0u)			postprocess = postprocess * random(gl_FragCoord.xy + u_time);
	if ((u_parameters & INVERT) != 0u)			postprocess = vec4(1-postprocess);
	f_color = mix(color, postprocess, u_blend);
}