precision mediump float;

varying vec2 v_texcoord;
varying vec3 v_ambientColor;
varying vec3 v_diffuseColor;
varying vec3 v_specularColor;

varying float v_diffuse;

varying float v_shininess;
varying vec3 v_viewDir;
varying vec3 v_reflection;

uniform sampler2D s_texture;

void main() {	
	vec3 ambient_color = v_ambientColor;
	
	//calc diffuse
	float diffuse = clamp(v_diffuse, 0.0, 1.0);
	vec3 diffuse_color = v_diffuseColor * diffuse;
	
	//calc specular
	vec3 specular_color = vec3(0);
	if(diffuse > 0.0) {
		vec3 reflection = normalize(v_reflection);
		vec3 viewDir = normalize(v_viewDir);

		float dot_result = clamp(dot(reflection, -viewDir ), 0.0, 1.0);
		float pow_result = pow(dot_result, v_shininess);
		specular_color = v_specularColor * pow_result;
	}
	
	vec4 color = vec4(ambient_color + diffuse_color + specular_color, 1.0);
	//vec4 color = vec4(specular_color, 1.0);
	//gl_FragColor = color;
	gl_FragColor = color * texture2D(s_texture, v_texcoord);
}