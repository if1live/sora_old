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
#include <glm/glm.hpp>
#endif


namespace sora {;
//재질데이터는 MTL을 기반으로 적절히 수정되었다. 필요한 부분만 고쳣기 때문에 완벽하지는 않다
//원본 MTL데이터 포멧
//http://people.sc.fsu.edu/~jburkardt/data/mtl/mtl.html
struct Material {
  enum {
    kInvalidShaderFlag = UINT_MAX,
  };

  static const Material &GetInvalidMtl();
  Material();

  std::string name;
  uint uber_flag;

  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;  
  float alpha;   //tr or d
  float shininess;  //diffuse

  //빛과 섞어쓸 텍스쳐
  //단색 텍스쳐의 경우는 ambient = 1, 텍스쳐 1개로 흉내내기가 가능하니까 통합시켰다
  std::string ambient_map;
  std::string diffuse_map;
  std::string specular_map;
  std::string normal_map;

public:
  bool operator==(const Material &o) const;
  bool operator!=(const Material &o) const {
    return !(*this == o);
  }
};
}

#endif  // SORA_MATERIAL_H_
