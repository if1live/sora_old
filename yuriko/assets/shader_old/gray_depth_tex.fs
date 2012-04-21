//http://fabiensanglard.net/dEngineSourceCodeRelease/
precision mediump float;
varying vec2 v_texcoord;
uniform sampler2D s_texture;

void main() {
	vec4 position = texture2D(s_texture, v_texcoord);
	float dist = position.z / position.w;
	if(dist == 1.0) {
		gl_FragColor = vec4(0.0);
	} else {
		//float normalized_dist = (dist * 0.6) + 0.2;
		float normalized_dist = (1.0 - dist) * 100.0;
		gl_FragColor = vec4(vec3(normalized_dist), 1.0);
	}
}