uniform mat4 Projection;    
uniform mat4 Modelview;     
attribute vec4 Position;

#ifdef USE_COLOR    
attribute vec4 SourceColor;    
varying vec4 DestinationColor;
#endif

#ifdef USE_TEXCOORD
attribute vec2 TexCoord;    
varying vec2 DestinationTexCoord;     
#endif

void main()           
{
	//fragment shader로 정보 넘기기
	//uber shader version
#ifdef USE_COLOR
	DestinationColor = SourceColor; 
#endif
#ifdef USE_TEXCOORD
	DestinationTexCoord = TexCoord; 
#endif
	
	//common code
	gl_Position = Projection * Modelview * Position; 
}
