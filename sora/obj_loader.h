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
#pragma once

#include "globals.h"
#include "material.h"

namespace sora {;

//mtl / obj는 같은 형태를 공유하니까
//공유하는걸 적절히 만들엇다
class ObjFormatHelper {
public:
  //\n을 \0로 변환
  static void NewLineToNullChar(uchar *start, uchar *end);
  //\0을 기준으로 잘라서 문장의 시작을 의미하는 포인터 얻기
  static int GetLineStartPos(uchar *start, uchar *end, std::vector<uchar*> &line_list);
  static void GetLineLength(const std::vector<uchar*> &line_list, std::vector<int> &length_list);
};

class ObjLoader {
public:
  ObjLoader();
  ~ObjLoader();

  static int GetFaceVertexCount(int face_type, int elem_count);
  //void Load(uchar *start, uchar *end, ObjModel *model);
};

class MtlLoader {
public:
  MtlLoader();
  ~MtlLoader();

  void Load(uchar *start, uchar *end, std::vector<Material> *mtl_list);
  void Parse(uchar *str, int n);
private:
  std::vector<Material> *mtl_list_;
  Material *GetLastMtl();
};
} //namespace sora