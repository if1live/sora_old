precision mediump float;  
varying vec4 v_color;  
varying vec2 v_texcoord; 
uniform sampler2D s_texture; 
void main()  
{  
	vec4 tmp = texture2D(s_texture, v_texcoord); 
	vec4 tmp1 = vec4(tmp.x * v_color.x, tmp.y * v_color.y, tmp.z * v_color.z, tmp.w * v_color.w);
	float minimum = 0.3;
	float r = tmp1.x > minimum ? tmp1.x : minimum; 
	float g = tmp1.y > minimum ? tmp1.y : minimum; 
	float b = tmp1.z > minimum ? tmp1.z : minimum; 
	float a = tmp1.w > minimum ? tmp1.w : minimum; 
	gl_FragColor = vec4(r, g, b, a);  
}