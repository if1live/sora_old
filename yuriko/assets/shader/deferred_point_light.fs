precision mediump float;
varying vec2 v_texcoord;

uniform mat4 u_mvp3d;
uniform vec4 u_diffuseColor;
uniform vec4 u_specularColor;
uniform vec4 u_viewport;

uniform vec4 u_lightPos;
uniform float u_lightRadius;

uniform sampler2D s_viewSpaceNormal;
uniform sampler2D s_depth;
uniform sampler2D s_specularMap;
uniform sampler2D s_diffuseMap;

vec3 get_pos(float depth) {
	float scr_width = u_viewport.z;
	float scr_height = u_viewport.w;
	vec2 raw_pos = vec2(gl_FragCoord.x / scr_width, gl_FragCoord.y / scr_height);
	vec3 pos = vec3((raw_pos * 2.0) - vec2(1.0), depth);
	return pos;
}
/*
vec4 calc_diffuse(vec3 normal, out float diffuse_var) {
	//diffuse 적절히 계산하기
	float diffuse = dot(u_lightDir, normal);
	diffuse = clamp(diffuse, 0.0, 1.0);
	diffuse_var = diffuse;
	
	vec4 diffuse_color = u_diffuseColor * diffuse;
	vec4 diffuse_texel = texture2D(s_diffuseMap, v_texcoord);
	diffuse_color = diffuse_color * diffuse_texel;
	return diffuse_color;
}
vec4 calc_specular(vec3 normal, float depth) {
	vec4 specular_texel = texture2D(s_specularMap, v_texcoord);
	float shininess = specular_texel.w;
	
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
	specular_color *= vec4(specular_texel.xyz, 1.0);
	return specular_color;
}
*/
void main() {
	//0~1 => -1~+1
	vec3 normal = texture2D(s_viewSpaceNormal, v_texcoord).xyz;
	normal = (normal * 2.0) - vec3(1.0);
	float depth = texture2D(s_depth, v_texcoord).x;
	
	if(depth == 1.0) {
		gl_FragColor = vec4(0.0);
		return;
	}
	
	vec3 pos = get_pos(depth);
	
	/*
	
	if(pos.z > 1.0) {
		gl_FragColor = vec4(0.0, 0.0, 1.0, 1.0);
	} else if(pos.z > 0.97) {
		gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);
	} else if(pos.z > 0.968) {
		gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
	} else {
		gl_FragColor = vec4(1.0, 1.0, 0.0, 1.0);
	}*/
	
	vec4 light_pos = u_mvp3d * u_lightPos;
	float light_z = (light_pos.z / light_pos.w) + 0.021;	//why error??
	//if(pos.z < (light_pos.z / light_pos.w) + 0.01) {
	if(depth < light_z) {
		//lightz가 뒤에 잇으면 파란색
		gl_FragColor = vec4(0.0, 0.0, 1.0, 1.0);
		return;
	} else {
		gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);
		return;
	}
	
	/*
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
*/
	//gl_FragColor = vec4(0.0, pos.z, 0.0, 1.0);
}
