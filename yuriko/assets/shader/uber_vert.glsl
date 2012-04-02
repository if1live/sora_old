attribute vec4 a_position;
uniform mat4 u_worldViewProjection;

// TEXTURE_MAP : ambient/diffuse/specular map이 설정될 경우
// 각각은 텍스쳐 좌표를 아마도 공유할테니까 그떄 쓰자
// DIFFUSE_SPECULAR : diffuse/specular의 경우 계산로직 일부를 공유하니까 하나만 초기화되도 
// 활성화 시키도록함

//ambient
uniform vec4 u_ambientColor;
varying vec4 v_ambientColor;

//diffuse
uniform vec4 u_diffuseColor;
varying vec4 v_diffuseColor;
varying float v_diffuse;

//specular
uniform vec4 u_specularColor;
varying vec4 v_specularColor;
uniform float u_specularShininess;
varying float v_shininess;
varying vec3 v_reflection;
varying vec3 v_viewDir;

//light calc
uniform vec3 u_worldLightPosition;
uniform mat4 u_world;
attribute vec3 a_normal;
uniform vec4 u_viewPosition;

//for texture
attribute vec2 a_texcoord;
varying vec2 v_texcoord;

void main() {
#ifdef USE_AMBIENT
	bool use_ambient = true;
#else
	bool use_ambient = false;
#endif
#ifdef USE_AMBIENT_MAP
	bool use_ambient_map = true;
#else
	bool use_ambient_map = false;
#endif

#ifdef USE_DIFFUSE
	bool use_diffuse = true;
#else
	bool use_diffuse = false;
#endif
#ifdef USE_DIFFUSE_MAP
	bool use_diffuse_map = true;
#else
	bool use_diffuse_map = false;
#endif

#ifdef USE_SPECULAR
	bool use_specular = true;
#else
	bool use_specular = false;
#endif
#ifdef USE_SPECULAR_MAP
	bool use_specular_map = true;
#else
	bool use_specular_map = false;
#endif

	if(use_ambient) {
		v_ambientColor = u_ambientColor;
	}

	
	if(use_diffuse || use_specular) {
		vec3 light_dir = a_position.xyz - u_worldLightPosition.xyz;
		light_dir = normalize(light_dir);	
		mat3 u_world_3 = mat3(u_world[0].x, u_world[0].y, u_world[0].z,
						u_world[1].x, u_world[1].y, u_world[1].z,
						u_world[2].x, u_world[2].y, u_world[2].z);
		vec3 world_normal = u_world_3 * a_normal;
		world_normal = normalize(world_normal);
		
		if(use_diffuse) {
			v_diffuseColor = u_diffuseColor;
			v_diffuse = dot(-light_dir, world_normal);
		}

		if(use_specular) {
			v_specularColor = u_specularColor;
			v_shininess = u_specularShininess;
			v_viewDir = normalize(a_position.xyz - u_viewPosition.xyz);
			v_reflection = reflect(light_dir, world_normal);
		}
	}
		
	if(use_ambient_map || use_diffuse_map || use_specular_map) {
		v_texcoord = a_texcoord;
	}

	gl_Position = u_worldViewProjection * a_position;
}