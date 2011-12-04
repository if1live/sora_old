// Copyright (C) 2011 by if1live
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
// Ŭnicode please
#include "sora_stdafx.h"
#include "sora/math_helper.h"

namespace sora {;

mat4 &SetOrtho(float left, float right, float bottom, float top, float near_val, float far_val, mat4 *m) {
	float a = static_cast<float>(2) / (right - left);
	float b = static_cast<float>(2) / (top - bottom);
	float c = static_cast<float>(2) / (far_val - near_val);
	float tx = (right+left) / (right-left);
	float ty = (top+bottom) / (top-bottom);
	float tz = (far_val+near_val) / (far_val-near_val);

	float data[] = {
		a, 0, 0, 0,
		0, b, 0, 0, 
		0, 0, c, 0,
		-tx, -ty, -tz, 1
	};
  m->Set(data);
  return *m;
}

mat4 &SetFrustum(float left, float right, float bottom, float top, float near_val, float far_val, mat4 *m) {
	float a = static_cast<float>(2) * near_val / (right - left);
	float b = static_cast<float>(2) * near_val / (top - bottom);
	float c = (right + left) / (right - left);
	float d = (top + bottom) / (top - bottom);
	float e = - (far_val + near_val) / (far_val - near_val);
	float f = -static_cast<float>(2) * far_val * near_val / (far_val - near_val);
	float data[] = {
		a, 0, 0, 0,
		0, b, 0, 0,
		c, d, e, -1,
		0, 0, f, 1
	};
  m->Set(data);
  return *m;
}

mat4 &SetLookAt(float eye_x, float eye_y, float eye_z,
  float target_x, float target_y, float target_z,
  float up_x, float up_y, float up_z, mat4 *m) {
  vec3 eye(eye_x, eye_y, eye_z);
  vec3 target(target_x, target_y, target_z);
  vec3 up(up_x, up_y, up_z);
  vec3 z = Normalize(eye-target);
  vec3 x = Normalize(Cross(up, z));
  vec3 y = Normalize(Cross(z, x));

  float data[4];
  data[0] = x.x;
  data[1] = x.y;
  data[2] = x.z;
  data[3] = 0;
  m->SetCol(0, data);

	data[0] = y.x;
  data[1] = y.y;
  data[2] = y.z;
  data[3] = 0;
  m->SetCol(1, data);

	data[0] = z.x;
  data[1] = z.y;
  data[2] = z.z;
  data[3] = 0;
  m->SetCol(2, data);

	data[0] = 0;
  data[1] = 0;
  data[2] = 0;
  data[3] = 1;
  m->SetCol(3, data);

  vec4 eye_prime = (*m) * vec4(-eye, 1);
  
  mat4 result = m->Transpose();
  result.SetCol(3, (float[4])eye_prime.value);

	*m = result;
  return *m;
}

mat4 &SetPerspective(float fovy, float aspect, float zNear, float zFar, mat4 *m) {
	///http://www.opengl.org/wiki/GluPerspective_code
	float ymax, xmax;
	//float temp, temp2, temp3, temp4;
	ymax = zNear * tanf((float)(fovy * kPi / 360.0));
	//ymin = -ymax;
	//xmin = -ymax * aspectRatio;
	xmax = ymax * aspect;
	return SetFrustum(-xmax, xmax, -ymax, ymax, zNear, zFar, m);
}
}