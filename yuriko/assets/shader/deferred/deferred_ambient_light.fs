precision mediump float;
varying vec2 v_texcoord;
uniform vec4 u_ambientColor;
uniform sampler2D s_diffuseMap;
void main() {
	vec4 diffuse_color = texture2D(s_diffuseMap, v_texcoord);
	gl_FragColor = diffuse_color * u_ambientColor;
}