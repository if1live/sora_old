uniform mat4 u_mvp;

//model data
attribute vec4 a_position;

attribute vec4 a_color;
varying vec4 v_color;

attribute vec3 a_normal;
varying vec3 v_normal;

//for texture
attribute vec2 a_texcoord;
varying vec2 v_texcoord;

varying vec3 v_position;

void main() {
	const bool use_ambient = AMBIENT_MASK == 1 ? true : false;
	const bool use_diffuse = DIFFUSE_MASK == 1 ? true : false;
	const bool use_diffuse_map = DIFFUSE_MAP_MASK == 1 ? true : false;
	const bool use_specular = SPECULAR_MASK == 1 ? true : false;
	const bool use_specular_map = SPECULAR_MAP_MASK == 1 ? true : false;
	const bool use_normal_map = NORMAL_MAP_MASK == 1 ? true : false;

	vec4 position = u_mvp * a_position;
	gl_Position = position;
	v_position = position.xyz;
	v_color = a_color;	
	v_texcoord = a_texcoord;
	v_normal = a_normal;
}