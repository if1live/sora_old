precision mediump float;

#ifdef USE_TEXTURE
varying vec2 v_texcoord;
uniform sampler2D s_texture;
#endif

#ifdef USE_MODEL_COLOR
varying vec4 v_color;
#endif

#ifdef USE_CONST_COLOR
varying vec4 v_constColor;
#endif

void main() {
	vec4 color = vec4(1.0);
#ifdef USE_CONST_COLOR
	color = v_constColor;	//base color
#endif
#ifdef USE_MODEL_COLOR
	color = v_color;
#endif

#ifdef USE_TEXTURE
	color = color * texture2D(s_texture, v_texcoord);
#endif
	gl_FragColor =  color;
}