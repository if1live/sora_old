precision mediump float;

varying vec4 v_color;
varying vec2 v_texcoord;
varying vec3 v_normal;

uniform sampler2D s_diffuseMap;

void main() {
/*
	const bool use_ambient = AMBIENT_MASK == 1 ? true : false;
	const bool use_diffuse = DIFFUSE_MASK == 1 ? true : false;
	const bool use_diffuse_map = DIFFUSE_MAP_MASK == 1 ? true : false;
	const bool use_specular = SPECULAR_MASK == 1 ? true : false;
	const bool use_specular_map = SPECULAR_MAP_MASK == 1 ? true : false;
	const bool use_normal_map = NORMAL_MAP_MASK == 1 ? true : false;
*/	
	//RT0 : view-space normal
	//RT1 : diffuse(albedo * diffuse * normal color)
	gl_FragData[0] = vec4(v_normal, 1.0);
	gl_FragData[1] = v_color;
}
