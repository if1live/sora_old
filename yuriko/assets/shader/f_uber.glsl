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

//텍스쳐 좌표같은거 관련 속성
#ifdef USE_AMBIENT_MAP
	#ifndef TEXTURE_MAP
	#define TEXTURE_MAP
	#endif
uniform sampler2D s_ambientMap;
#endif
#ifdef USE_DIFFUSE_MAP
	#ifndef TEXTURE_MAP
	#define TEXTURE_MAP
	#endif
uniform sampler2D s_diffuseMap;
#endif
#ifdef USE_SPECULAR_MAP
	#ifndef TEXTURE_MAP
	#define TEXTURE_MAP
	#endif
uniform sampler2D s_specularMap;
#endif

#ifdef TEXTURE_MAP
varying vec2 v_texcoord;
#endif

void main() {
	vec4 color = vec4(0.0);

	/* calc ambient */
#ifdef USE_AMBIENT
	{
		vec4 ambient_color = v_ambientColor;
#ifdef USE_AMBIENT_MAP
		vec4 ambient_tex = texture2D(s_ambientMap, v_texcoord);
		ambient_color = ambient_color * ambient_tex;
#endif
		color = color + ambient_color;
	}
#endif
	/* calc ambient */

	/* calc diffuse */
#ifdef USE_DIFFUSE
	{
		float diffuse = clamp(v_diffuse, 0.0, 1.0);
		vec4 diffuse_color = v_diffuseColor * diffuse;
#ifdef USE_DIFFUSE_MAP
		vec4 diffuse_tex = texture2D(s_diffuseMap, v_texcoord);
		diffuse_color = diffuse_color * diffuse_tex;
#endif
		color = color + diffuse_color;
	}
#endif
	/* calc diffuse */

	/* calc specular */
#ifdef USE_SPECULAR
	{
		//calc specular
		vec3 reflection = normalize(v_reflection);
		vec3 viewDir = normalize(v_viewDir);

		float dot_result = clamp(dot(reflection, -viewDir), 0.0, 1.0);
		float pow_result = pow(dot_result, v_shininess);
		vec4 specular_color = v_specularColor * pow_result;
		
	#ifdef USE_SPECULAR_MAP
		vec4 specular_tex = texture2D(s_specularMap, v_texcoord);
		specular_color = specular_color * specular_tex;
	#endif
		color = color + specular_color;
	}
#endif
	/* calc specular */
	
	gl_FragColor =  color;
}