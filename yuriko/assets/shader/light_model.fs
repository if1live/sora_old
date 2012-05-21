precision mediump float;
uniform vec4 u_diffuseColor;
uniform vec4 u_specularColor;

vec4 calc_diffuse(vec3 normal, vec3 light_dir, vec4 color, out float diffuse_var) {
	//diffuse 적절히 계산하기
	float diffuse = dot(light_dir, normal);
	diffuse = clamp(diffuse, 0.0, 1.0);
	diffuse_var = diffuse;
	
	vec4 diffuse_color = u_diffuseColor * diffuse;
	diffuse_color *= color;
	return diffuse_color;
}

vec4 calc_specular(vec3 normal, vec3 light_dir, vec3 color, float shininess, vec3 view_dir) {
	vec3 reflection = reflect(light_dir, normal);
	reflection = normalize(reflection);

	float dot_result = clamp(dot(reflection, view_dir), 0.0, 1.0);
	if(dot_result == 0) {
		return vec4(0.0);
	}
	float pow_result = pow(dot_result, shininess);
	vec4 specular_color = u_specularColor * pow_result;
	//specular_color *= vec4(color, 1.0);
	return specular_color;
}