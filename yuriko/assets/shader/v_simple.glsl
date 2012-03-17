attribute vec4 a_position;
attribute vec2 a_texcoord;
uniform mat4 u_modelViewProjection;

varying vec2 v_texcoord;
void main() {
  v_texcoord = a_texcoord;
  gl_Position = u_modelViewProjection * a_position;
}