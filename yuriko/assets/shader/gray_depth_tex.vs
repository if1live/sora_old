//ȸ������ ���̸��� ǥ���ϸ� �ð������� �˾ƺ��� ���ϴ�
attribute vec4 a_position;
attribute vec2 a_texcoord;
varying vec2 v_texcoord;

uniform mat4 u_worldViewProjection;

void main() {
	v_texcoord = a_texcoord;
	gl_Position = u_worldViewProjection * a_position;
}