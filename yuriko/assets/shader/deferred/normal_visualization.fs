precision mediump float;
varying vec2 v_texcoord;
uniform sampler2D s_texture;
void main() {
	//-1~+1
	vec3 normal = texture2D(s_texture, v_texcoord).xyz;
	normal = (normal + vec3(1.0)) / 2.0;
	gl_FragColor = vec4(normal, 1.0);
}