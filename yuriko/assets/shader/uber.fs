precision mediump float;

//ambient
varying vec4 v_ambientColor;

//diffuse
varying vec4 v_diffuseColor;
varying float v_diffuse;

//specular
varying vec4 v_specularColor;
varying float v_shininess;
varying vec3 v_reflection;
varying vec3 v_viewDir;

//ambient/diffuse/specular map
uniform sampler2D s_ambientMap;
uniform sampler2D s_diffuseMap;
uniform sampler2D s_specularMap;
varying vec2 v_texcoord;

void main() {
	vec4 color = vec4(0.0);
	
#if AMBIENT_MASK == 1
	bool use_ambient = true;
#else
	bool use_ambient = false;
#endif
#if AMBIENT_MAP_MASK == 1
	bool use_ambient_map = true;
#else
	bool use_ambient_map = false;
#endif

#if DIFFUSE_MASK == 1
	bool use_diffuse = true;
#else
	bool use_diffuse = false;
#endif
#if DIFFUSE_MAP_MASK == 1
	bool use_diffuse_map = true;
#else
	bool use_diffuse_map = false;
#endif

#if SPECULAR_MASK == 1
	bool use_specular = true;
#else
	bool use_specular = false;
#endif
#if SPECULAR_MAP_MASK == 1
	bool use_specular_map = true;
#else
	bool use_specular_map = false;
#endif

	if(use_ambient) {
		vec4 ambient_color = v_ambientColor;
		if(use_ambient_map) {
			vec4 ambient_tex = texture2D(s_ambientMap, v_texcoord);
			ambient_color = ambient_color * ambient_tex;
		}
		color = color + ambient_color;
	}

	if(use_diffuse) {
		float diffuse = clamp(v_diffuse, 0.0, 1.0);
		vec4 diffuse_color = v_diffuseColor * diffuse;
		if(use_diffuse_map) {
			vec4 diffuse_tex = texture2D(s_diffuseMap, v_texcoord);
			diffuse_color = diffuse_color * diffuse_tex;
		}
		color = color + diffuse_color;
	}

	if(use_specular) {
		//calc specular
		vec3 reflection = normalize(v_reflection);
		vec3 viewDir = normalize(v_viewDir);

		float dot_result = clamp(dot(reflection, -viewDir), 0.0, 1.0);
		float pow_result = pow(dot_result, v_shininess);
		vec4 specular_color = v_specularColor * pow_result;
		
		if(use_specular_map) {
			vec4 specular_tex = texture2D(s_specularMap, v_texcoord);
			specular_color = specular_color * specular_tex;
		}
		color = color + specular_color;
	}
	
	gl_FragColor =  color;
}