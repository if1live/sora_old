uniform mat4 u_mvp;

attribute vec4 a_position;
attribute vec2 a_texcoord;
attribute vec3 a_viewVector;

varying vec2 v_texcoord;
varying vec3 v_viewVector;

void main() {
	v_texcoord = a_texcoord;
	v_viewVector = a_viewVector;
	gl_Position = u_mvp * a_position;
}