precision mediump float;

varying ivec4 v_colorId;

void main() {
	float x = v_colorId.x / 255.0;
	float y = v_colorId.y / 255.0;
	float z = v_colorId.z / 255.0;
	float w = v_colorId.w / 255.0;
	gl_FragColor = vec4(x, y, z, w);
}