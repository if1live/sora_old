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
#ifndef SORA_MATERIAL_MANAGER_H_
#define SORA_MATERIAL_MANAGER_H_

#include "template_lib.h"
#include "material.h"

#if SR_USE_PCH == 0
#include <string>
#include <vector>
#endif

namespace sora {;
class MaterialManager : public Singleton<MaterialManager> {
public:
  MaterialManager();
  ~MaterialManager();

public:
  //이름중복이 발생한 경우 false
  bool Add(const std::vector<Material> &mtl_list);
  bool Add(const Material &mtl);
  bool IsExist(const std::string &name) const;
  const Material &Get(const std::string &name);
  void Clear();

private:
  //일단은 몇개 안될테니까 간단하게 구현
  //재질정보는 그렇게 크지도 않고 많지도 않을테니까 전부 떄려박아도 심각한 문제가
  //발생하지는 않을것이다.
  std::vector<Material> material_list_;
};

//어차피 자주쓸 메모스를 GetInstance로 거쳐서 받는것도 삽질같아서 그냥 함수 뚫음
SR_C_DLL const Material &MaterialMgr_get(const std::string &name);
SR_C_DLL bool MaterialMgr_is_exist(const std::string &name);

//initialize material list
SR_C_DLL bool MaterialMgr_initialize_from_file();
}


#endif  // SORA_MATERIAL_MANAGER_H_