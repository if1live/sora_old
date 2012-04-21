uniform mat4 u_worldViewProjection;

//model data
attribute vec4 a_position;

attribute vec4 a_color;
varying vec4 v_color;

attribute vec3 a_normal;
varying vec3 v_normal;

attribute vec3 a_tangent;
varying vec3 v_tangent;

//for texture
attribute vec2 a_texcoord;
varying vec2 v_texcoord;

//light calc
uniform vec3 u_worldLightPosition;
uniform mat4 u_world;
uniform vec4 u_viewPosition;

varying mat3 v_world;
varying vec3 v_viewDir;
varying vec3 v_lightDir;

void main() {
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

	gl_Position = u_worldViewProjection * a_position;
	v_color = a_color;
	v_tangent = a_tangent;
	v_normal = a_normal;
	v_texcoord = a_texcoord;

	if(use_diffuse || use_specular) {
		vec3 light_dir = a_position.xyz - u_worldLightPosition.xyz;
		v_lightDir = normalize(light_dir);	
		v_viewDir = normalize(a_position.xyz - u_viewPosition.xyz);
		
		v_world = mat3(u_world[0].x, u_world[0].y, u_world[0].z,
						u_world[1].x, u_world[1].y, u_world[1].z,
						u_world[2].x, u_world[2].y, u_world[2].z);
	}
}