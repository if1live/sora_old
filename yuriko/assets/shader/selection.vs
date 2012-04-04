attribute vec4 a_position;
uniform mat4 u_worldViewProjection;

uniform ivec4 u_colorId;
varying ivec4 v_colorId;

void main() {
	v_colorId = u_colorId;
	gl_Position = u_worldViewProjection * a_position;
}