precision mediump float;

#ifdef USE_COLOR
varying vec4 DestinationColor;	//varying lowp vec4 DestinationColor;
#endif

#ifdef USE_TEXCOORD
varying vec2 DestinationTexCoord;	//varying lowp vec2 DestinationTexCoord;
uniform sampler2D Texture;
#endif

void main()                                  
{
#ifdef USE_COLOR
	gl_FragColor = DestinationColor;
#endif

#ifdef USE_TEXCOORD
	gl_FragColor = texture2D(Texture, DestinationTexCoord);
#endif
}