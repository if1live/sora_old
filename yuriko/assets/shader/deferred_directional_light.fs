precision mediump float;
varying vec2 v_texcoord;

uniform vec4 u_diffuseColor;
uniform vec4 u_specularColor;
uniform vec3 u_lightDir;

uniform sampler2D s_viewSpaceNormal;
uniform sampler2D s_depth;

vec4 calc_diffuse(vec3 normal) {
	//diffuse 적절히 계산하기
	float diffuse = dot(-u_lightDir, normal);
	diffuse = clamp(diffuse, 0.0, 1.0);
	vec4 diffuse_color = u_diffuseColor * diffuse;
	return diffuse_color;
}

void main() {
	//0~1 => -1~+1
	vec3 normal = texture2D(s_viewSpaceNormal, v_texcoord).xyz;
	normal = (normal * 2.0) - vec3(1.0);
	
	float depth = texture2D(s_depth, v_texcoord).x;
	
	if(depth == 1.0) {
		gl_FragColor = vec4(0.0);
		return;
	}
	
	vec4 color = vec4(0.0);
	color = color + calc_diffuse(normal);
	gl_FragColor = color;
}
