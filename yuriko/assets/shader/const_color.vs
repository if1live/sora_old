attribute vec4 a_position;
uniform mat4 u_modelViewProjection;
void main() {
  gl_Position = u_modelViewProjection * a_position;
}