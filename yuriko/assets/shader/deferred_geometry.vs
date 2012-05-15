uniform mat4 u_modelViewProjection;
uniform mat3 u_model3_InverseTranspose;

//model data
attribute vec4 a_position;

attribute vec4 a_color;
varying vec4 v_color;

attribute vec3 a_normal;
varying vec3 v_normal;

//for texture
attribute vec2 a_texcoord;
varying vec2 v_texcoord;

void main() {
	gl_Position = u_modelViewProjection * a_position;
	v_color = a_color;
	v_normal = u_model3_InverseTranspose * a_normal;
	v_texcoord = a_texcoord;	
}