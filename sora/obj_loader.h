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
#ifndef SORA_OBJ_LOADER_H_
#define SORA_OBJ_LOADER_H_

#include "vector.h"
#if SR_USE_PCH == 0
#include <vector>
#endif

namespace sora {;

struct ObjModel;
struct Material;

struct ObjLoaderImpl;
//http://people.sc.fsu.edu/~jburkardt/data/mtl/mtl.html
class SR_DLL ObjLoader {
public:
  //파싱을 하는데 사용되는 스태틱 함수 모음
  friend class ObjModel;
  friend class Material;
  //\n을 \0로 변환
  static void NewLineToNullChar(uchar *start, uchar *end);
  //\0을 기준으로 잘라서 문장의 시작을 의미하는 포인터 얻기
  static int GetLineStartPos(uchar *start, uchar *end, std::vector<uchar*> &line_list);
  static void GetLineLength(const std::vector<uchar*> &line_list, std::vector<int> &length_list);
  
  static int GetFaceVertexCount(int face_type, int elem_count);

public:
  ObjLoader();
  ~ObjLoader();
  void LoadObj(uchar *start, uchar *end, ObjModel *model);
  void LoadMtl(uchar *start, uchar *end, std::vector<Material> *mtl_list);

private:
  enum {
    kFaceVertex,
    kFaceVertexTex,
    kFaceVertexTexNormal,
    kFaceVertexNormal
  };

private:
  void ParseObjLine(uchar *str, int n);
  void ParseMtlLine(uchar *str, int n);

private:
  ObjLoaderImpl *impl;
};

}

#endif  // SORA_OBJ_LOADER_H_