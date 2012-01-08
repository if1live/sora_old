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

#if SR_ANDROID
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif

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
  float up_x, float up_y, float up_z, mat4 *result) {
    /*
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
  */

     GLfloat m[16];
        GLfloat x[3], y[3], z[3];
        GLfloat mag;
        
        /* Make rotation matrix */
        
        /* Z vector */
        z[0] = eye_x - target_x;
        z[1] = eye_y - target_y;
        z[2] = eye_z - target_z;
        mag = sqrt(z[0] * z[0] + z[1] * z[1] + z[2] * z[2]);
        if (mag) {                      /* mpichler, 19950515 */
                z[0] /= mag;
                z[1] /= mag;
                z[2] /= mag;
        }
        
        /* Y vector */
        y[0] = up_x;
        y[1] = up_y;
        y[2] = up_z;
        
        /* X vector = Y cross Z */
        x[0] = y[1] * z[2] - y[2] * z[1];
        x[1] = -y[0] * z[2] + y[2] * z[0];
        x[2] = y[0] * z[1] - y[1] * z[0];
        
        /* Recompute Y = Z cross X */
        y[0] = z[1] * x[2] - z[2] * x[1];
        y[1] = -z[0] * x[2] + z[2] * x[0];
        y[2] = z[0] * x[1] - z[1] * x[0];
        
        /* mpichler, 19950515 */
        /* cross product gives area of parallelogram, which is < 1.0 for
         * non-perpendicular unit-length vectors; so normalize x, y here
         */
        
        mag = sqrt(x[0] * x[0] + x[1] * x[1] + x[2] * x[2]);
        if (mag) {
                x[0] /= mag;
                x[1] /= mag;
                x[2] /= mag;
        }
        
        mag = sqrt(y[0] * y[0] + y[1] * y[1] + y[2] * y[2]);
        if (mag) {
                y[0] /= mag;
                y[1] /= mag;
                y[2] /= mag;
        }
        
#define M(row,col)  m[col*4+row]
        M(0, 0) = x[0];
        M(0, 1) = x[1];
        M(0, 2) = x[2];
        M(0, 3) = 0.0;
        M(1, 0) = y[0];
        M(1, 1) = y[1];
        M(1, 2) = y[2];
        M(1, 3) = 0.0;
        M(2, 0) = z[0];
        M(2, 1) = z[1];
        M(2, 2) = z[2];
        M(2, 3) = 0.0;
        M(3, 0) = 0.0;
        M(3, 1) = 0.0;
        M(3, 2) = 0.0;
        M(3, 3) = 1.0;
#undef M

        sora::mat4 m1(m);
        sora::mat4 m2;
        sora::SetTranslate(-eye_x, -eye_y, -eye_z, &m2);
        m1 *= m2;
        *result = m1;
        return *result;
        //glMultMatrixf(m);
        /* Translate Eye to Origin */
        //glTranslatef(-eyex, -eyey, -eyez);
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