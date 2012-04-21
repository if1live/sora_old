attribute vec4 a_position;
attribute vec3 a_normal;
uniform mat4 u_worldViewProjection;

uniform vec4 u_constColor;
varying vec4 v_constColor;

void main() {
	v_constColor = u_constColor;
	vec3 tmp_pos = a_position.xyz + (a_normal * 0.0);
	vec4 new_pos = vec4(tmp_pos, 1);
	gl_Position = u_worldViewProjection * new_pos;
}