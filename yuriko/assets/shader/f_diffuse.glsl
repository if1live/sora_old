precision mediump float;

varying vec2 v_texcoord;
varying vec3 v_ambientColor;
varying vec3 v_diffuseColor;
varying float v_diffuse;

uniform sampler2D s_texture;

void main() {	
	vec3 ambient_color = v_ambientColor;
	
	//calc diffuse
	float diffuse = clamp(v_diffuse, 0.0, 1.0);
	vec3 diffuse_color = v_diffuseColor * diffuse;
	
	vec4 color = vec4(ambient_color + diffuse_color, 1.0);
	gl_FragColor = color * texture2D(s_texture, v_texcoord);
}