precision mediump float;

#ifdef USE_AMBIENT
varying vec4 v_ambientColor;
#endif

#ifdef USE_DIFFUSE
varying vec4 v_diffuseColor;
varying float v_diffuse;
#endif

#ifdef USE_SPECULAR
varying vec4 v_specularColor;
varying float v_shininess;
varying vec3 v_reflection;
varying vec3 v_viewDir;
#endif

#ifdef USE_DIFFUSE_MAP
#ifndef DIFFUSE_SPECULAR_MAP
#define DIFFUSE_SPECULAR_MAP 1
#endif
uniform sampler2D s_diffuseMap;
#endif

#ifdef USE_SPECULAR_MAP
#ifndef DIFFUSE_SPECULAR_MAP
#define DIFFUSE_SPECULAR_MAP 1
#endif
uniform sampler2D s_specularMap;
#endif

#ifdef DIFFUSE_SPECULAR_MAP
varying vec2 v_texcoord;
#endif

void main() {
	vec4 color = vec4(0.0);

#ifdef USE_AMBIENT
	color = color + v_ambientColor;
#endif

	vec4 diffuse_factor = vec4(1.0);
#ifdef USE_DIFFUSE_MAP
	diffuse_factor = texture2D(s_diffuseMap, v_texcoord);
#endif
#ifdef USE_DIFFUSE
	float diffuse = clamp(v_diffuse, 0.0, 1.0);
	vec4 diffuse_color = v_diffuseColor * diffuse;
	color = color + (diffuse_color * diffuse_factor);
#endif

	vec4 specular_factor = vec4(1.0);
#ifdef USE_SPECULAR_MAP
	specular_factor = texture2D(s_specularMap, v_texcoord);
#endif
#ifdef USE_SPECULAR
	//calc specular
	vec4 specular_color = v_specularColor;
	//if(diffuse > 0.0) {
	vec3 reflection = normalize(v_reflection);
	vec3 viewDir = normalize(v_viewDir);

	float dot_result = clamp(dot(reflection, -viewDir), 0.0, 1.0);
	float pow_result = pow(dot_result, v_shininess);
	specular_color = v_specularColor * pow_result;
	//}
	color = color + (specular_color * specular_factor);
#endif
	gl_FragColor =  color;
}