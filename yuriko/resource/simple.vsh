uniform mat4 u_projection;
uniform mat4 u_modelview;
attribute vec3 a_position;
void main() {
	gl_Position = u_projection * u_modelview * vec4(a_position, 1.0); 
}