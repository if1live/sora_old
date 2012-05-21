precision mediump float;
varying vec2 v_texcoord;
uniform sampler2D s_texture;

//http://www.geeks3d.com/20091216/geexlab-how-to-visualize-the-depth-buffer-in-glsl/

void main() {
	//-1~+1
	float depth = texture2D(s_texture, v_texcoord).x;
	gl_FragColor = vec4(depth, depth, depth, 1.0);
}