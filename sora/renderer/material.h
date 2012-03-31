/*  Copyright (C) 2011-2012 by if1live */
//
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
#ifndef SORA_MATERIAL_H_
#define SORA_MATERIAL_H_

#if SR_USE_PCH == 0
#include <cstring>
#endif

namespace sora {;
//재질데이터는 MTL을 기반으로 일단 구현한다. 나중에 다른 포맷에 맞춰서 확장하자
struct Material {
  Material() {
    //http://people.sc.fsu.edu/~jburkardt/data/mtl/mtl.html
    //mtl format default value
    for(int i = 0 ; i < 3 ; i++) {
      ambient[i] = 0.2f;
      diffuse[i] = 0.8f;
      specular[i] = 1.0f;
    }
    alpha = 1.0f;  //not transparent
    shininess = 0.0f;

    uber_flag = 0;
  };
  std::string name;

  float ambient[3];
  float diffuse[3];
  float specular[3];
  
  float alpha;   //tr or d
  float shininess;
  
  uint uber_flag;

  //단색으로 쓸떄의 텍스쳐
  std::string tex_map;   

  //빛과 섞어쓸 텍스쳐
  std::string diffuse_map;
  std::string specular_map;
  std::string normal_map;
};
}

#endif  // SORA_MATERIAL_H_
