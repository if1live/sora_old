//http://fabiensanglard.net/dEngineSourceCodeRelease/
precision mediump float;
varying vec2 v_texcoord;
uniform sampler2D s_texture;

// This is the shadowmap generator shader
const vec4 packFactors = vec4( 256.0 * 256.0 * 256.0,256.0 * 256.0,256.0,1.0);
const vec4 bitMask     = vec4(0.0,1.0/256.0,1.0/256.0,1.0/256.0);

void main() {
	vec4 position = texture2D(s_texture, v_texcoord);
	float normalizedDistance  = position.z / position.w;
	normalizedDistance = (normalizedDistance + 1.0) / 2.0;

	vec4 packedValue = vec4(fract(packFactors*normalizedDistance));
	packedValue -= packedValue.xxyz * bitMask;

	gl_FragColor  = packedValue;
}