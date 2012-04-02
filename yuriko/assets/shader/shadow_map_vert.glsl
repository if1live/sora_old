attribute vec4 a_position;
uniform mat4 u_worldViewProjection;
uniform mat3 u_world;
uniform vec3 u_lightPosition;
uniform float u_far;

varying vec3 v_lightDir;
varying float v_far;

void main() {
	vec3 vert_pos = u_world * a_position.xyz;
	v_lightDir = u_lightPosition - vert_pos;
	v_far = u_far;
	gl_Position = u_worldViewProjection * a_position;
}