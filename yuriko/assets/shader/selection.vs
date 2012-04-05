attribute vec4 a_position;
uniform mat4 u_worldViewProjection;

uniform ivec4 u_colorId;
varying vec4 v_colorId;

void main() {
	vec4 tmp_color_id = vec4(u_colorId);
	tmp_color_id = tmp_color_id / 255.0;
	v_colorId = tmp_color_id;
	gl_Position = u_worldViewProjection * a_position;
}