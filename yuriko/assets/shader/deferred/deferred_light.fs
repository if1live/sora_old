precision mediump float;
varying vec2 v_texcoord;
varying vec3 v_viewVector;

uniform mat4 u_projectionInv;
uniform mat4 u_mvpInv;
uniform mat4 u_mv;
uniform mat4 u_view;
uniform vec4 u_viewPosition;

uniform vec2 u_clipPlane;

#ifdef POINT_LIGHT
//4번째 속성이 반지름
uniform vec4 u_lightPos;
#endif
#ifdef DIRECTION_LIGHT
uniform vec3 u_lightDir;
#endif

uniform vec4 u_viewport;

uniform sampler2D s_viewSpaceNormal;
uniform sampler2D s_depth;
uniform sampler2D s_specularMap;
uniform sampler2D s_diffuseMap;
uniform sampler2D s_positionMap;

vec4 unproject(float depth) {
	mat4 inverse = u_mvpInv;
	vec3 win = vec3(gl_FragCoord.xy, depth);
	vec4 tmp = vec4(win, 1.0);
	tmp.x = (tmp.x - (u_viewport[0])) / (u_viewport[2]);
	tmp.y = (tmp.y - (u_viewport[1])) / (u_viewport[3]);
	tmp = (tmp * 2.0) - vec4(1.0);

	vec4 obj = inverse * tmp;
	return obj;
}

vec3 get_ndc_pos(float depth) {
//http://stackoverflow.com/questions/5669287/opengl-compute-eye-space-coord-from-window-space-coord-in-glsl
	vec3 ndc_pos;
	ndc_pos.xy = gl_FragCoord.xy / u_viewport.zw;
	ndc_pos.z = depth; // or gl_FragCoord.z
	ndc_pos -= 0.5;
	ndc_pos *= 2.0;
	return ndc_pos;
}

vec4 get_eye_pos(float depth) {
	vec3 ndc_pos = get_ndc_pos(depth);
	float z_near = u_clipPlane.x;
	float z_far = u_clipPlane.y;
	
	float a = (z_near + z_far);
	float b = (z_near - z_far);
	float c = (2.0 * z_near * z_far);
	float d = (ndc_pos.z * b);
	
	vec4 clip_pos;
	clip_pos.w = -(-(c / (a + d)));
	clip_pos.xyz = ndc_pos * clip_pos.w;
	vec4 eye_pos = u_projectionInv * clip_pos;
	return eye_pos;
}
/*
vec3 get_viewspace_pixel_pos(float depth) {
	vec2 raw_pos = gl_FragCoord.xy / u_viewport.zw;	//0~1
	vec2 pixel_xy = (raw_pos * 2.0) - vec2(1.0);	
	
	//http://stackoverflow.com/questions/5669287/opengl-compute-eye-space-coord-from-window-space-coord-in-glsl
	vec4 v_screen = vec4(pixel_xy, depth, 1.0 );
	vec4 v_homo = u_projectionInv * 2.0*(v_screen-vec4(0.5));
	vec3 v_eye = v_homo.xyz / v_homo.w;	//transfer from homogeneous coordinates
	//added code
	v_eye.xy /= u_viewport.zw;
	v_eye.z *= -1;
	return v_eye;
}
*/

#ifdef POINT_LIGHT
vec4 point_lighting(float depth, vec3 light_pos, float light_radius, vec3 n) {
	vec4 specular_texel = texture2D(s_specularMap, v_texcoord);
	float shininess = specular_texel.w * 255;
	vec4 diffuse_texel = texture2D(s_diffuseMap, v_texcoord);
	vec3 world_pos = texture2D(s_positionMap, v_texcoord).xyz;
	
	vec3 light_dir = light_pos - world_pos;
	float light_dist = length(light_dir);
	if(light_dist > light_radius) {
		return vec4(0.0, 0.0, 0.0, 1.0);
	}
	
	//반지름 기반으로적당한 감쇄 적용
	float att_0 = 1.0;
	float att_1 = 1.0;
	float normalized_light_dist = light_dist / light_radius;
	float att = 1.0 / (att_0 + att_1 * normalized_light_dist);
	
	vec3 view_dir = u_viewPosition.xyz - world_pos;
	
	vec3 viewspace_light_dir = mat3(u_view) * light_dir;
	vec3 viewspace_view_dir = mat3(u_view) * view_dir;
	
	viewspace_light_dir = normalize(viewspace_light_dir);
	viewspace_view_dir = normalize(viewspace_view_dir);
	
	float diffuse_var = 0;
	vec4 diffuse_color = calc_diffuse(n, viewspace_light_dir, diffuse_texel, diffuse_var);
	vec4 specular_color = vec4(0.0);
	if(diffuse_var > 0.0) {
		specular_color = calc_specular(n, viewspace_light_dir, specular_texel.xyz, shininess, viewspace_view_dir);
	}
	vec4 color = vec4(0.0);
	color = color + diffuse_color;
	color = color + specular_color;
	return color * att;
	
}
#endif

#ifdef DIRECTION_LIGHT
vec4 direction_light(float depth, vec3 n) {
	vec4 specular_texel = texture2D(s_specularMap, v_texcoord);
	float shininess = specular_texel.w * 255;
	vec4 diffuse_texel = texture2D(s_diffuseMap, v_texcoord);
	vec3 world_pos = texture2D(s_positionMap, v_texcoord).xyz;
	
	vec3 light_dir = u_lightDir;
	vec3 view_dir = u_viewPosition.xyz - world_pos;
	
	vec3 viewspace_light_dir = mat3(u_view) * light_dir;
	vec3 viewspace_view_dir = mat3(u_view) * view_dir;
	
	viewspace_light_dir = normalize(viewspace_light_dir);
	viewspace_view_dir = normalize(viewspace_view_dir);
	
	float diffuse_var = 0;
	vec4 diffuse_color = calc_diffuse(n, viewspace_light_dir, diffuse_texel, diffuse_var);
	vec4 specular_color = vec4(0.0);
	if(diffuse_var > 0.0) {
		specular_color = calc_specular(n, viewspace_light_dir, specular_texel.xyz, shininess, viewspace_view_dir);
	}
	vec4 color = vec4(0.0);
	//color = color + diffuse_color;
	color = color + specular_color;
	return color;
	//return vec4(n, 1.0);
}
#endif

void main() {
	//0~1 => -1~+1
	vec3 normal = texture2D(s_viewSpaceNormal, v_texcoord).xyz;
	float depth = texture2D(s_depth, v_texcoord).x;
	
	if(depth == 1.0) {
		gl_FragColor = vec4(0.0);
		return;
	}

#ifdef POINT_LIGHT
	vec3 light_pos = u_lightPos.xyz;
	float radius = u_lightPos.w;
	vec4 color = point_lighting(depth, light_pos, radius, normal);
	//vec4 color = vec4(light_pos.xyz, 1.0);
#endif
#ifdef DIRECTION_LIGHT
	vec4 color = direction_light(depth, normal);
#endif
	gl_FragColor = color;
}
