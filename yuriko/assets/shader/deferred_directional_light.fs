precision mediump float;
varying vec2 v_texcoord;

uniform vec4 u_diffuseColor;
uniform vec4 u_specularColor;
uniform vec3 u_lightDir;

uniform sampler2D s_viewSpaceNormal;
uniform sampler2D s_depth;
uniform sampler2D s_specularMap;

vec4 calc_diffuse(vec3 normal, out float diffuse_var) {
	//diffuse 적절히 계산하기
	float diffuse = dot(u_lightDir, normal);
	diffuse = clamp(diffuse, 0.0, 1.0);
	vec4 diffuse_color = u_diffuseColor * diffuse;
	diffuse_var = diffuse;
	return diffuse_color;
}
vec4 calc_specular(vec3 normal, float depth) {
	float shininess = texture2D(s_specularMap, v_texcoord).w;
	//vec3 pos = texture2D(s_pos, v_texcoord).xyz;
	vec2 raw_pos = vec2(gl_FragCoord.x / 640.0, gl_FragCoord.y / 480.0);	//0~1
	vec3 pos = vec3((raw_pos * 2.0) - vec2(1.0), depth);
	vec3 view_dir = -normalize(pos);
	
	vec3 reflection = reflect(u_lightDir, normal);
	reflection = normalize(reflection);

	float dot_result = clamp(dot(reflection, view_dir), 0.0, 1.0);
	if(dot_result == 0) {
		return vec4(0.0);
	}
	float pow_result = pow(dot_result, shininess);
	vec4 specular_color = u_specularColor * pow_result;
	return specular_color;
}

void main() {
	//0~1 => -1~+1
	vec3 normal = texture2D(s_viewSpaceNormal, v_texcoord).xyz;
	normal = (normal * 2.0) - vec3(1.0);
	float depth = texture2D(s_depth, v_texcoord).x;
	
	
	if(depth == 1.0) {
		gl_FragColor = vec4(0.0);
		return;
	}
	
	float diffuse_var = 0.0;
	vec4 diffuse_color = calc_diffuse(normal, diffuse_var);
	vec4 specular_color = vec4(0.0);
	if(diffuse_var > 0.0) {
		specular_color = calc_specular(normal, depth);
	}
	
	vec4 color = vec4(0.0);
	color = color + diffuse_color;
	color = color + specular_color;
	gl_FragColor = color;
}
