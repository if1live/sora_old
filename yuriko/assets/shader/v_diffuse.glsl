attribute vec4 a_position;
attribute vec2 a_texcoord;
attribute vec3 a_normal;
uniform mat4 u_modelViewProjection;

uniform vec3 u_ambientColor;
uniform vec3 u_diffuseColor;

varying vec2 v_texcoord;
varying vec3 v_ambientColor;
varying vec3 v_diffuseColor;
varying float v_diffuse;

void main() {
	v_texcoord = a_texcoord;
	v_ambientColor = u_ambientColor;
	v_diffuseColor = u_diffuseColor;
	
	//calc diffuse
	vec3 light_pos = vec3(0, 0, 100);
	vec3 light_dir = a_position.xyz - light_pos.xyz;
	light_dir = normalize(light_dir);

	//mat3 u_world_3 = mat3(u_world[0].x, u_world[0].y, u_world[0].z,
	//				u_world[1].x, u_world[1].y, u_world[1].z,
	//				u_world[2].x, u_world[2].y, u_world[2].z);
	mat3 u_world_3 = mat3(1.0, 0.0, 0.0,  0.0, 1.0, 0.0,  0.0, 0.0, 1.0);
	vec3 world_normal = a_normal * u_world_3;
	world_normal = normalize(world_normal);
	v_diffuse = dot(-light_dir, world_normal);

	gl_Position = u_modelViewProjection * a_position;
}