precision mediump float;
varying vec2 v_texcoord;
uniform sampler2D s_texture;
void main() {
	vec4 color = texture2D(s_texture, v_texcoord);
	float gray = color.r * 0.299 + color.g * 0.587 + color.b * 0.114;
	gl_FragColor = vec4(gray, gray, gray, color.w);
}