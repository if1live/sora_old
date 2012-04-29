precision mediump float;

varying vec4 v_color;
varying vec2 v_texcoord;
varying vec3 v_normal;
varying vec3 v_tangent;

uniform vec4 u_ambientColor;
uniform vec4 u_diffuseColor;
uniform vec4 u_specularColor;
uniform float u_specularShininess;

varying vec3 v_viewDir;
varying vec3 v_lightDir;
varying mat3 v_model;

//ambient/diffuse/specular map
uniform sampler2D s_ambientMap;
uniform sampler2D s_diffuseMap;
uniform sampler2D s_specularMap;
uniform sampler2D s_normalMap;

void main() {
#if AMBIENT_MASK == 1
	const bool use_ambient = true;
#else
	const bool use_ambient = false;
#endif
#if AMBIENT_MAP_MASK == 1
	const bool use_ambient_map = true;
#else
	const bool use_ambient_map = false;
#endif

#if DIFFUSE_MASK == 1
	const bool use_diffuse = true;
#else
	const bool use_diffuse = false;
#endif
#if DIFFUSE_MAP_MASK == 1
	const bool use_diffuse_map = true;
#else
	const bool use_diffuse_map = false;
#endif

#if SPECULAR_MASK == 1
	const bool use_specular = true;
#else
	const bool use_specular = false;
#endif
#if SPECULAR_MAP_MASK == 1
	const bool use_specular_map = true;
#else
	const bool use_specular_map = false;
#endif

#if NORMAL_MAP_MASK == 1
	const bool use_normal_map = true;
#else
	const bool use_normal_map = false;
#endif

	vec4 color = vec4(0.0);
	vec3 normal = v_normal;

	if(use_normal_map) {
		//�ؽ��Ŀ��� �������� �̱�
		highp vec3 tangent_space_normal = texture2D(s_normalMap, v_texcoord).yxz * 2.0 - 1.0;
		highp vec3 n = v_normal;
		highp vec3 t = normalize(v_tangent);
		highp vec3 b = normalize(cross(n, t));
		
		highp mat3 basis = mat3(n, t, b);
		highp vec3 N = basis * tangent_space_normal;
		normal = N;
	}
	if(use_ambient) {
		vec4 ambient_color = u_ambientColor;
		if(use_ambient_map) {
			vec4 ambient_tex = texture2D(s_ambientMap, v_texcoord);
			ambient_color = ambient_color * ambient_tex;
		}
		color = color + ambient_color;
	}
	
	if(use_diffuse || use_specular) {
		vec3 model_normal = v_model * normal;
		model_normal = normalize(model_normal);
		
		if(use_diffuse) {
			float diffuse = dot(-v_lightDir, model_normal);
			diffuse = clamp(diffuse, 0.0, 1.0);
			vec4 diffuse_color = u_diffuseColor * diffuse;
			if(use_diffuse_map) {
				vec4 diffuse_tex = texture2D(s_diffuseMap, v_texcoord);
				diffuse_color = diffuse_color * diffuse_tex;
			}
			color = color + diffuse_color;
		}

		if(use_specular) {
			vec3 reflection = reflect(v_lightDir, model_normal);
			//calc specular
			reflection = normalize(reflection);
			vec3 viewDir = normalize(v_viewDir);

			float dot_result = clamp(dot(reflection, -viewDir), 0.0, 1.0);
			float pow_result = pow(dot_result, u_specularShininess);
			vec4 specular_color = u_specularColor * pow_result;
			
			if(use_specular_map) {
				vec4 specular_tex = texture2D(s_specularMap, v_texcoord);
				specular_color = specular_color * specular_tex;
			}
			color = color + specular_color;
		}
	}
	
	gl_FragColor =  color * v_color;
}