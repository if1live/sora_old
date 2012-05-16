uniform mat4 u_modelViewProjection;

//model data
attribute vec4 a_position;

attribute vec4 a_color;
varying vec4 v_color;

attribute vec3 a_normal;
varying vec3 v_normal;

attribute vec3 a_tangent;
varying vec3 v_tangent;

//for texture
attribute vec2 a_texcoord;
varying vec2 v_texcoord;

//light calc
uniform vec3 u_modelLightPosition;
uniform mat4 u_modelInverseTranspose;
uniform vec4 u_viewPosition;

varying mat3 v_modelInverseTranspose;
varying vec3 v_viewDir;
varying vec3 v_lightDir;

void main() {
	const bool use_ambient = AMBIENT_MASK == 1 ? true : false;
	const bool use_diffuse = DIFFUSE_MASK == 1 ? true : false;
	const bool use_diffuse_map = DIFFUSE_MAP_MASK == 1 ? true : false;
	const bool use_specular = SPECULAR_MASK == 1 ? true : false;
	const bool use_specular_map = SPECULAR_MAP_MASK == 1 ? true : false;

	gl_Position = u_modelViewProjection * a_position;
	v_color = a_color;
	v_tangent = a_tangent;
	v_normal = a_normal;
	v_texcoord = a_texcoord;

	if(use_diffuse || use_specular) {
		vec3 light_dir = a_position.xyz - u_modelLightPosition.xyz;
		v_lightDir = normalize(light_dir);	
		v_viewDir = normalize(a_position.xyz - u_viewPosition.xyz);
		
		v_modelInverseTranspose = mat3(
			u_modelInverseTranspose[0].x, u_modelInverseTranspose[0].y, u_modelInverseTranspose[0].z,
			u_modelInverseTranspose[1].x, u_modelInverseTranspose[1].y, u_modelInverseTranspose[1].z,
			u_modelInverseTranspose[2].x, u_modelInverseTranspose[2].y, u_modelInverseTranspose[2].z
		);
	}
}
