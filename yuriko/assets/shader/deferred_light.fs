precision mediump float;
varying vec2 v_texcoord;

uniform mat4 u_projectionInv;
uniform vec4 u_diffuseColor;
uniform vec4 u_specularColor;
uniform vec2 u_clipPlane;

#ifdef POINT_LIGHT
uniform vec4 u_lightPos;
uniform float u_lightRadius;
#endif
#ifdef DIRECTION_LIGHT
uniform vec3 u_lightDir;
#endif

uniform vec4 u_viewport;

uniform sampler2D s_viewSpaceNormal;
uniform sampler2D s_depth;
uniform sampler2D s_specularMap;
uniform sampler2D s_diffuseMap;

#if POINT_LIGHT
vec3 get_light_pos() {
	//float light_z = u_lightPos.z * - 1;
	float light_z = u_lightPos.z;
	light_z += 0.021;	//why error??	
	return vec3(u_lightPos.xy, light_z);
}
#endif

vec3 get_viewspace_pixel_pos(float depth) {
	float scr_width = u_viewport.z;
	float scr_height = u_viewport.w;
	vec2 raw_pos = vec2(gl_FragCoord.x / scr_width, gl_FragCoord.y / scr_height);	//0~1
	vec2 pixel_xy = (raw_pos * 2.0) - vec2(1.0);	
	//return vec3(pixel_xy, -depth);
	vec4 view_pos = u_projectionInv * vec4(pixel_xy, -depth, 1.0);
	view_pos /= view_pos.w;
	return view_pos.xyz;
}

vec3 get_view_dir(float depth) {
	vec3 viewspace_pixel = get_viewspace_pixel_pos(depth);
	return -normalize(viewspace_pixel);
/*
	float scr_width = u_viewport.z;
	float scr_height = u_viewport.w;
	vec2 raw_pos = vec2(gl_FragCoord.x / scr_width, gl_FragCoord.y / scr_height);	//0~1
	vec2 pixel_xy = (raw_pos * 2.0) - vec2(1.0);	
	vec3 view_dir = vec3(-pixel_xy, -depth);
	return view_dir;
	*/
}

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

void main() {
	//0~1 => -1~+1
	vec3 normal = texture2D(s_viewSpaceNormal, v_texcoord).xyz;
	normal = (normal * 2.0) - vec3(1.0);
	float depth = texture2D(s_depth, v_texcoord).x;
	
	if(depth == 1.0) {
		gl_FragColor = vec4(0.0);
		return;
	}
	
	float diffuse_var = 0.0;
#ifdef POINT_LIGHT
	vec3 pixel_pos = get_viewspace_pixel_pos(depth);
	vec3 light_pos = get_light_pos();
	vec3 light_dir = normalize(light_pos - pixel_pos);
	vec3 view_dir = -normalize(pixel_pos);
#endif
#ifdef DIRECTION_LIGHT
	vec3 light_dir = u_lightDir;
	vec3 view_dir = get_view_dir(depth);
#endif
	
	vec4 diffuse_color = calc_diffuse(normal, light_dir, diffuse_var);
	vec4 specular_color = vec4(0.0);
	if(diffuse_var > 0.0) {
		specular_color = calc_specular(normal, light_dir, view_dir);
	}
	
	vec4 color = vec4(0.0);
	//color = color + diffuse_color;
	color = color + specular_color;
	gl_FragColor = color;
	
	//gl_FragColor = get_viewspace_pixel_pos(depth);
}
