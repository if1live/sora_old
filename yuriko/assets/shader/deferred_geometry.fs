precision mediump float;

varying vec4 v_color;
varying vec2 v_texcoord;
varying vec3 v_normal;

uniform sampler2D s_diffuseMap;

void main() {
	//RT0 : view-space normal
	//RT1 : diffuse	
	gl_FragData[0] = vec4(v_normal, 1.0);
	gl_FragData[1] = v_color;
}
