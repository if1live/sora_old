uniform mat4 u_mvp;  
attribute vec4 a_position; 
attribute vec2 a_texcoord; 
attribute vec4 a_color;      
varying vec4 v_color;  
varying vec2 v_texcoord; 
void main()  
{  
	v_texcoord = a_texcoord; 
	v_color = a_color; 
	gl_Position = u_mvp * a_position;
}