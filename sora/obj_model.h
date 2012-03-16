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
#ifndef SORA_OBJ_MODEL_H_
#define SORA_OBJ_MODEL_H_

#include "vertex.h"
namespace sora {;

class MemoryFile;
class ObjModel;
class ObjModelLoader;

struct SR_DLL ObjModel {
  std::vector<Vertex> vert_list;
  // GL_TRIANGLES로 그릴수 잇도록 정렬된 인덱스 리스트
  std::vector<ushort> index_list;
};

//파싱을 하는데 사용되는 스태틱 함수 모음
class SR_DLL ObjModelLoader {
public:
  friend class ObjModel;
  //\n을 \0로 변환
  static void NewLineToNullChar(uchar *start, uchar *end);
  //\0을 기준으로 잘라서 문장의 시작을 의미하는 포인터 얻기
  static int GetLineStartPos(uchar *start, uchar *end, std::vector<uchar*> &line_list);
  static void GetLineLength(const std::vector<uchar*> &line_list, std::vector<int> &length_list);
  
  static int GetFaceVertexCount(int face_type, int elem_count);

public:
  ObjModelLoader();
  ~ObjModelLoader();
  void Load(uchar *start, uchar *end, ObjModel *model);

private:
  enum {
    kFaceVertex,
    kFaceVertexTex,
    kFaceVertexTexNormal,
    kFaceVertexNormal
  };

private:
  void ParseLine(uchar *str, int n);

private:
  std::vector<Vec3f> pos_list_;
  std::vector<Vec2f> tex_list_;
  std::vector<Vec3f> normal_list_;

  ObjModel *model_;
};

}

#endif  // SORA_OBJ_MODEL_H_