precision mediump float;
varying vec2 v_texcoord;
varying vec3 v_viewVector;

uniform mat4 u_projectionInv;
uniform mat4 u_mvpInv;
uniform mat4 u_mv;

uniform vec4 u_diffuseColor;
uniform vec4 u_specularColor;
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
vec4 calc_diffuse(vec3 normal, vec3 light_dir, out float diffuse_var) {
	//diffuse 적절히 계산하기
	float diffuse = dot(light_dir, normal);
	diffuse = clamp(diffuse, 0.0, 1.0);
	diffuse_var = diffuse;
	
	vec4 diffuse_color = u_diffuseColor * diffuse;
	vec4 diffuse_texel = texture2D(s_diffuseMap, v_texcoord);
	diffuse_color = diffuse_color * diffuse_texel;
	return diffuse_color;
}

vec4 calc_specular(vec3 normal, vec3 light_dir, vec3 view_dir) {
	vec4 specular_texel = texture2D(s_specularMap, v_texcoord);
	float shininess = specular_texel.w * 255;
	
	vec3 reflection = reflect(light_dir, normal);
	reflection = normalize(reflection);

	float dot_result = clamp(dot(reflection, view_dir), 0.0, 1.0);
	if(dot_result == 0) {
		return vec4(0.0);
	}
	float pow_result = pow(dot_result, shininess);
	vec4 specular_color = u_specularColor * pow_result;
	//specular_color *= vec4(specular_texel.xyz, 1.0);
	return specular_color;
}

#ifdef POINT_LIGHT
vec4 point_lighting(float depth, vec3 light_pos, float light_radius, vec3 p, vec3 n) {
	vec3 light_dir = light_pos - p;	//light vec
	vec3 view_dir = normalize(p);	//view vec
	
	float diffuse_var = 0;
	vec4 diffuse_color = calc_diffuse(n, light_dir, diffuse_var);
	vec4 specular_color = vec4(0.0);
	if(diffuse_var > 0.0) {
		specular_color = calc_specular(n, light_dir, view_dir);
	}
	vec4 color = vec4(0.0);
	color = color + diffuse_color;
	color = color + specular_color;
	return color;
}
#endif

#ifdef DIRECTION_LIGHT
vec4 direction_light(float depth, vec3 n) {
	vec3 light_dir = u_lightDir;
	//vec3 view_dir = -normalize(get_ndc_pos(depth));
	vec3 view_dir = v_viewVector;
	float diffuse_var = 0;
	vec4 diffuse_color = calc_diffuse(n, light_dir, diffuse_var);
	vec4 specular_color = vec4(0.0);
	if(diffuse_var > 0.0) {
		specular_color = calc_specular(n, light_dir, view_dir);
	}
	vec4 color = vec4(0.0);
	color = color + diffuse_color;
	color = color + specular_color;
	return color;
}
#endif

void main() {
	//0~1 => -1~+1
	vec3 normal = texture2D(s_viewSpaceNormal, v_texcoord).xyz;
	normal = (normal * 2.0) - vec3(1.0);
	float depth = texture2D(s_depth, v_texcoord).x;
	
	if(depth == 1.0) {
		gl_FragColor = vec4(0.0);
		return;
	}

#ifdef POINT_LIGHT
	vec3 pos;
	pos.z = u_clipPlane.y / (u_clipPlane.x + depth);
	pos.xy = view.xy/view.z*pos.z;
	
	//view dir
	vec3 light_pos = u_lightPos.xyz;
	float radius = u_lightPos.w;
	vec4 color = point_lighting(depth, light_pos, radius, pos, normal) {
#endif
#ifdef DIRECTION_LIGHT
	vec4 color = direction_light(depth, normal);
#endif
	gl_FragColor = color;
	
#ifdef POINT_LIGHT
	vec4 viewpos = unproject(depth);
	viewpos *= viewpos.w;
	viewpos *= 0.01;
	//viewpos.z *= -1;
	gl_FragColor = viewpos;
	//gl_FragColor = get_eye_pos(depth);
#endif
}
