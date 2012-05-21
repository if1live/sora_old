precision mediump float;

uniform mat3 u_rotationMat;
uniform mat4 u_mv;

varying vec4 v_color;
varying vec2 v_texcoord;
varying vec3 v_normal;
varying vec3 v_position;

uniform vec4 u_ambientColor;
uniform vec4 u_diffuseColor;
uniform vec4 u_specularColor;
uniform float u_specularShininess;

uniform sampler2D s_diffuseMap;
uniform sampler2D s_specularMap;

vec4 calc_specular_color() {
	const bool use_specular = SPECULAR_MASK == 1 ? true : false;
	const bool use_specular_map = SPECULAR_MAP_MASK == 1 ? true : false;
	
	if(use_specular == false) {
		return vec4(0.0);
	}
	
	vec4 specular_color = u_specularColor;
	if(use_specular_map) {
		vec4 specular_texel = texture2D(s_specularMap, v_texcoord);
		specular_color = specular_color * specular_texel;
	}
	specular_color.w = u_specularShininess / 255.0;
	return specular_color;
}

vec4 calc_diffuse_color() {
	const bool use_diffuse = DIFFUSE_MASK == 1 ? true : false;
	const bool use_diffuse_map = DIFFUSE_MAP_MASK == 1 ? true : false;
	
	if(use_diffuse == false) {
		return vec4(0.0);
	}
	vec4 diffuse_color = u_diffuseColor;
	if(use_diffuse_map) {
		vec4 diffuse_texel = texture2D(s_diffuseMap, v_texcoord);
		diffuse_color = diffuse_color * diffuse_texel;
	}
	return diffuse_color;
}

vec3 calc_viewspace_normal() {
	const bool use_normal_map = NORMAL_MAP_MASK == 1 ? true : false;
	vec3 normal = u_rotationMat * v_normal;
	return normalize(normal);
}

void main() {
	//RT0 : view-space normal
	vec3 normal = calc_viewspace_normal();
	gl_FragData[0] = vec4(normal, 1.0);
	
	//RT1 : diffuse(albedo * diffuse * normal color)
	vec4 diffuse_color = calc_diffuse_color();
	gl_FragData[1] = diffuse_color;
	
	//RT2 : specular(grayscale + shininess)
	vec4 specular_color = calc_specular_color();
	gl_FragData[2] = specular_color;
	
	//RT3 : pos tex
	vec4 viewspace_pos = u_mv * vec4(v_position, 1.0);
	viewspace_pos /= viewspace_pos.w;
	//viewspace_pos.xyz = vec3(1.0) / viewspace_pos.xyz;
	gl_FragData[3] = vec4(viewspace_pos.xyz, 1.0);
}
