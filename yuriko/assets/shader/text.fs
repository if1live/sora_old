precision mediump float;
varying vec2 v_texcoord;
uniform sampler2D s_texture;
uniform vec4 u_constColor;
void main() {
	vec4 color = texture2D(s_texture, v_texcoord);
	if(color.w == 0.0) {
		gl_FragColor = vec4(0.0, 0.0, 0.0, color.w);
	} else {
		gl_FragColor = u_constColor;
	}
}