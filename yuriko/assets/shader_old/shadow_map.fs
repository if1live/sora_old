precision mediump float;

varying vec3 v_lightDir;
varying float v_far;

void main() {
	float dist = length(v_lightDir);
	/*
	//1���� ª�� �Ÿ��� ���, x=0, y=z=w=dist
	//1���� ū �Ÿ��� ���, x=1, y=z=w=1/dist
	if(dist > 1) {
		float w = 1.0 / dist;
		gl_FragColor = vec4(1, w, w, w);
	} else {
		gl_FragColor = vec4(0, dist, dist, dist);
	}
	*/
	dist = 1.0 - (dist / v_far);
	gl_FragColor = vec4(dist);
}
