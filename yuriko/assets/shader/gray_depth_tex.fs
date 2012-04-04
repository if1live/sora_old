//http://fabiensanglard.net/dEngineSourceCodeRelease/
precision mediump float;
varying vec2 v_texcoord;
uniform sampler2D s_texture;

void main() {
	vec4 position = texture2D(s_texture, v_texcoord);
	float dist = position.z / position.w;
	
	gl_FragColor = vec4((1 - dist) * 100);
}