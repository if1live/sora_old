precision mediump float;

uniform mat4 u_mvInvTranspose;

varying vec4 v_color;
varying vec2 v_texcoord;
varying vec3 v_normal;

uniform vec4 u_ambientColor;
uniform vec4 u_diffuseColor;
uniform vec4 u_specularColor;
uniform float u_specularShininess;

uniform sampler2D s_diffuseMap;
uniform sampler2D s_specularMap;

void main() {
	const bool use_ambient = AMBIENT_MASK == 1 ? true : false;
	const bool use_diffuse = DIFFUSE_MASK == 1 ? true : false;
	const bool use_diffuse_map = DIFFUSE_MAP_MASK == 1 ? true : false;
	const bool use_specular = SPECULAR_MASK == 1 ? true : false;
	const bool use_specular_map = SPECULAR_MAP_MASK == 1 ? true : false;
	const bool use_normal_map = NORMAL_MAP_MASK == 1 ? true : false;

	//RT0 : view-space normal
	vec4 normal4 = u_mvInvTranspose * vec4(v_normal, 1.0);
	//normal의 범위는 -1~+1인데 저장은 0~+1만 되니까 적절히 정규화하기
	vec3 normal = (normal4.xyz + vec3(1.0)) * 0.5;
	gl_FragData[0] = vec4(normal, 1.0);
	
	
	//RT1 : diffuse(albedo * diffuse * normal color)
	vec4 color = vec4(0.0);
	if(use_ambient) {
		vec4 ambient_color = u_ambientColor;
		if(use_diffuse == false && use_diffuse_map) {
			vec4 diffuse_texel = texture2D(s_diffuseMap, v_texcoord);
			ambient_color = ambient_color * diffuse_texel;
		}
		color = color + ambient_color;
	}
	if(use_diffuse) {
		vec4 diffuse_color = u_diffuseColor;
		if(use_diffuse_map) {
			vec4 diffuse_texel = texture2D(s_diffuseMap, v_texcoord);
			diffuse_color = diffuse_color * diffuse_texel;
		}
		color = color + diffuse_color;
	}
	gl_FragData[1] = color;
	
	
	
	//RT2 : specular(grayscale + shininess)
	vec4 specular_color = vec4(0.0);
	if(use_specular) {
		specular_color = u_specularColor;
		if(use_specular_map) {
			vec4 specular_texel = texture2D(s_specularMap, v_texcoord);
			specular_color = specular_color * specular_texel;
		}
		specular_color.w = u_specularShininess;
	}
	gl_FragData[2] = specular_color;
}
