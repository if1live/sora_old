precision mediump float;
varying vec2 v_texcoord;
uniform sampler2D s_texture;

uniform vec2 u_clipPlane;
//http://www.geeks3d.com/20091216/geexlab-how-to-visualize-the-depth-buffer-in-glsl/

void main() {
	//-1~+1
	float depth = texture2D(s_texture, v_texcoord).x;
	gl_FragColor = vec4(vec3(depth), 1.0);
}