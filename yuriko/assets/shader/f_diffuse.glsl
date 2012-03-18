precision mediump float;

varying vec2 v_texcoord;
varying vec4 v_ambientColor;
varying vec4 v_diffuseColor;
varying vec4 v_specularColor;

varying float v_diffuse;

varying float v_shininess;
varying vec3 v_viewDir;
varying vec3 v_reflection;

uniform sampler2D s_texture;

void main() {	
	vec4 ambient_color = v_ambientColor;
	
	//calc diffuse
	float diffuse = clamp(v_diffuse, 0.0, 1.0);
	vec4 diffuse_color = v_diffuseColor * diffuse;
	
	//calc specular
	vec4 specular_color = v_specularColor;
	//if(diffuse > 0.0) {
		vec3 reflection = normalize(v_reflection);
		vec3 viewDir = normalize(v_viewDir);

		float dot_result = clamp(dot(reflection, viewDir), 0.0, 1.0);
		float pow_result = pow(dot_result, v_shininess);
		specular_color = v_specularColor * pow_result;
	//}
	
	//vec4 color = ambient_color + diffuse_color + specular_color;
	//gl_FragColor = specular_color;
	gl_FragColor =  texture2D(s_texture, v_texcoord);
}