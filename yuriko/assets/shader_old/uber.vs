uniform mat4 u_worldViewProjection;

//model data
attribute vec4 a_position;
attribute vec3 a_normal;

attribute vec4 a_color;
varying vec4 v_color;

//for texture
attribute vec2 a_texcoord;
varying vec2 v_texcoord;

// TEXTURE_MAP : ambient/diffuse/specular map이 설정될 경우
// 각각은 텍스쳐 좌표를 아마도 공유할테니까 그떄 쓰자
// DIFFUSE_SPECULAR : diffuse/specular의 경우 계산로직 일부를 공유하니까 하나만 초기화되도 
// 활성화 시키도록함

//diffuse
varying float v_diffuse;

//specular
varying vec3 v_reflection;
varying vec3 v_viewDir;

//light calc
uniform vec3 u_worldLightPosition;
uniform mat4 u_world;
uniform vec4 u_viewPosition;

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

	v_color = a_color;
	
	if(use_diffuse || use_specular) {
		vec3 light_dir = a_position.xyz - u_worldLightPosition.xyz;
		light_dir = normalize(light_dir);	
		mat3 u_world_3 = mat3(u_world[0].x, u_world[0].y, u_world[0].z,
						u_world[1].x, u_world[1].y, u_world[1].z,
						u_world[2].x, u_world[2].y, u_world[2].z);
		vec3 world_normal = u_world_3 * a_normal;
		world_normal = normalize(world_normal);
		
		if(use_diffuse) {
			v_diffuse = dot(-light_dir, world_normal);
		}

		if(use_specular) {
			v_viewDir = normalize(a_position.xyz - u_viewPosition.xyz);
			v_reflection = reflect(light_dir, world_normal);
		}
	}
	if(use_ambient_map || use_diffuse_map || use_specular_map) {
		v_texcoord = a_texcoord;
	}

	gl_Position = u_worldViewProjection * a_position;
}