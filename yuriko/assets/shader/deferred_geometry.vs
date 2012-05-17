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
	vec4 position = u_mvp * a_position;
	gl_Position = position;
	v_position = position.xyz;
	v_color = a_color;	
	v_texcoord = a_texcoord;
	v_normal = a_normal;
}