precision mediump float;

#ifdef USE_TEXTURE
varying vec2 v_texcoord;
uniform sampler2D s_texture;
#endif

#ifdef USE_CONST_COLOR
varying vec4 v_constColor;
#endif

#ifdef USE_AMBIENT
#ifndef LIGHT_ENABLED
#define LIGHT_ENABLED 1
#endif
varying vec4 v_ambientColor;
#endif

#ifdef USE_DIFFUSE
#ifndef LIGHT_ENABLED
#define LIGHT_ENABLED 1
#endif
varying vec4 v_diffuseColor;
varying float v_diffuse;
#endif

#ifdef USE_SPECULAR
#ifndef LIGHT_ENABLED
#define LIGHT_ENABLED 1
#endif
varying vec4 v_specularColor;
varying float v_shininess;
varying vec3 v_reflection;
varying vec3 v_viewDir;
#endif

void main() {	
#ifdef LIGHT_ENABLED
	vec4 color = vec4(0.0);
#else
	vec4 color = vec4(1.0);
#endif

#ifdef USE_AMBIENT
	color = color + v_ambientColor;
#endif

#ifdef USE_DIFFUSE
	float diffuse = clamp(v_diffuse, 0.0, 1.0);
	vec4 diffuse_color = v_diffuseColor * diffuse;
	color = color + diffuse_color;
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
	color = color + specular_color;
#endif

#ifdef USE_CONST_COLOR
	color = v_constColor;	//base color
#endif
#ifdef USE_TEXTURE
	color = color * texture2D(s_texture, v_texcoord);
#endif
	gl_FragColor =  color;
}