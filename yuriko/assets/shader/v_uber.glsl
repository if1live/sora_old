attribute vec4 a_position;
uniform mat4 u_worldViewProjection;

// TEXTURE_MAP : ambient/diffuse/specular map�� ������ ���
// ������ �ؽ��� ��ǥ�� �Ƹ��� �������״ϱ� �׋� ����
// DIFFUSE_SPECULAR : diffuse/specular�� ��� ������ �Ϻθ� �����ϴϱ� �ϳ��� �ʱ�ȭ�ǵ� 
// Ȱ��ȭ ��Ű������
#ifdef USE_AMBIENT
uniform vec4 u_ambientColor;
varying vec4 v_ambientColor;
#endif

#ifdef USE_DIFFUSE
	#ifndef DIFFUSE_SPECULAR
	#define DIFFUSE_SPECULAR 1
	#endif
uniform vec4 u_diffuseColor;
varying vec4 v_diffuseColor;
varying float v_diffuse;
#endif

#ifdef USE_SPECULAR
	#ifndef DIFFUSE_SPECULAR
	#define DIFFUSE_SPECULAR 1
	#endif
uniform vec4 u_specularColor;
varying vec4 v_specularColor;
uniform float u_specularShininess;
varying float v_shininess;
varying vec3 v_reflection;
varying vec3 v_viewDir;
#endif

#ifdef DIFFUSE_SPECULAR
uniform vec3 u_worldLightPosition;
uniform mat4 u_world;
attribute vec3 a_normal;
uniform vec4 u_viewPosition;
#endif

//�ؽ��� ���� �Ӽ��� ����
#ifdef USE_AMBIENT_MAP
	#ifndef TEXTURE_MAP
	#define TEXTURE_MAP
	#endif
#endif
#ifdef USE_DIFFUSE_MAP
	#ifndef TEXTURE_MAP
	#define TEXTURE_MAP
	#endif
#endif
#ifdef USE_SPECULAR_MAP
	#ifndef TEXTURE_MAP
	#define TEXTURE_MAP
	#endif
#endif

#ifdef TEXTURE_MAP
attribute vec2 a_texcoord;
varying vec2 v_texcoord;
#endif


void main() {
#ifdef USE_AMBIENT
	v_ambientColor = u_ambientColor;
#endif

#ifdef DIFFUSE_SPECULAR
	vec3 light_dir = a_position.xyz - u_worldLightPosition.xyz;
	light_dir = normalize(light_dir);	
	mat3 u_world_3 = mat3(u_world[0].x, u_world[0].y, u_world[0].z,
					u_world[1].x, u_world[1].y, u_world[1].z,
					u_world[2].x, u_world[2].y, u_world[2].z);
	vec3 world_normal = u_world_3 * a_normal;
	world_normal = normalize(world_normal);
#endif

#ifdef TEXTURE_MAP
	v_texcoord = a_texcoord;
#endif

#ifdef USE_DIFFUSE
	v_diffuseColor = u_diffuseColor;
	v_diffuse = dot(-light_dir, world_normal);
#endif

#ifdef USE_SPECULAR
	v_specularColor = u_specularColor;
	v_shininess = u_specularShininess;
	v_viewDir = normalize(a_position.xyz - u_viewPosition.xyz);
	v_reflection = reflect(light_dir, world_normal);
#endif

	gl_Position = u_worldViewProjection * a_position;
}