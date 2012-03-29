attribute vec4 a_position;
uniform mat4 u_worldViewProjection;

#ifdef USE_TEXTURE
attribute vec2 a_texcoord;
varying vec2 v_texcoord;
#endif

#ifdef USE_CONST_COLOR
uniform vec4 u_constColor;
varying vec4 v_constColor;
#endif

#ifdef USE_MODEL_COLOR
attribute vec4 a_color;
varying vec2 v_color;
#endif

void main() {
#ifdef USE_TEXTURE
	v_texcoord = a_texcoord;
#endif
#ifdef USE_CONST_COLOR
	v_constColor = u_constColor;
#endif
#ifdef USE_MODEL_COLOR
	v_color = a_color;
#endif

	gl_Position = u_worldViewProjection * a_position;
}