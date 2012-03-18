attribute vec4 a_position;
attribute vec2 a_texcoord;
attribute vec3 a_normal;

uniform vec4 u_viewPosition;
uniform mat4 u_worldViewProjection;
uniform mat4 u_world;
uniform vec4 u_ambientColor;
uniform vec4 u_diffuseColor;
uniform vec4 u_specularColor;
uniform vec3 u_worldLightPosition;
uniform float u_specularShininess;

varying vec2 v_texcoord;
varying vec4 v_ambientColor;
varying vec4 v_diffuseColor;
varying vec4 v_specularColor;

varying float v_diffuse;

varying float v_shininess;
varying vec3 v_viewDir;
varying vec3 v_reflection;

void main() {
	v_texcoord = a_texcoord;
	v_ambientColor = u_ambientColor;
	v_diffuseColor = u_diffuseColor;
	v_specularColor = u_specularColor;
	v_shininess = u_specularShininess;
	
	//calc diffuse
	vec3 light_dir = a_position.xyz - u_worldLightPosition.xyz;
	light_dir = normalize(light_dir);
	
	v_viewDir = normalize(a_position.xyz - u_viewPosition.xyz);
	
	mat3 u_world_3 = mat3(u_world[0].x, u_world[0].y, u_world[0].z,
					u_world[1].x, u_world[1].y, u_world[1].z,
					u_world[2].x, u_world[2].y, u_world[2].z);
	vec3 world_normal = u_world_3 * a_normal;
	world_normal = normalize(world_normal);
	v_diffuse = dot(-light_dir, world_normal);
	
	
	//calc specular
	
	v_reflection = reflect(light_dir, world_normal);

	gl_Position = u_worldViewProjection * a_position;
}