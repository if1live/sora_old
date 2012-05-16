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
	v_texcoord = a_texcoord;	
	
	vec3 normal = u_model3_InverseTranspose * a_normal;
	//normal의 범위는 -1~+1인데 저장은 0~+1만 되니까 적절히 정규화하기
	v_normal = (normal + vec3(1.0)) * 0.5;
}