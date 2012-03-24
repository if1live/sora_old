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
#include "sora_stdafx.h"
#include "obj_loader.h"

#include "obj_model.h"
#include "material.h"

#include <boost/tokenizer.hpp>

using namespace std;

namespace sora {;

//pimpl pattern
struct ObjLoaderImpl {
  ObjLoaderImpl() : model(NULL), mtl_list(NULL) {}

  std::vector<Vec3f> pos_list;
  std::vector<Vec2f> tex_list;
  std::vector<Vec3f> normal_list;

  ObjModel *model;
  std::vector<Material> *mtl_list;

  Material *GetLastMtl() {
    SR_ASSERT(mtl_list != NULL);
    SR_ASSERT(mtl_list->size() > 0);
    return &mtl_list->back();
  }
};

void ObjLoader::NewLineToNullChar(uchar *start, uchar *end) {
  SR_ASSERT(start < end);
  SR_ASSERT(start != NULL);
  SR_ASSERT(end != NULL);
  //obj model은 한줄단위로 파싱하면 되니까 한줄의 경계를 확실하게 파악하기위해서 널을 썻다
  for(uchar *curr = start ; curr != end ; curr++) {
    if(*curr == '\n') {
      *curr = '\0';
    }
  }
}

int ObjLoader::GetLineStartPos(uchar *start, uchar *end, std::vector<uchar*> &line_list) {
  SR_ASSERT(start < end);
  SR_ASSERT(start != NULL);
  SR_ASSERT(end != NULL);
  SR_ASSERT(line_list.empty());
  if(*start == '\0') {
    //초장부터 널이 뜰리는 없겟지
    return 0;
  }

  uchar *curr = start;
  line_list.push_back(curr);
  for( ; curr != end ; curr++) {
    if(*curr == '\0') {
      uchar *next = curr + 1;
      if(*next != '\0') {
        line_list.push_back(next);
      }
    }
  }
  return line_list.size();
}

void ObjLoader::GetLineLength(const std::vector<uchar*> &line_list, std::vector<int> &length_list) {
  if(line_list.empty()) {
    return;
  }
  for(size_t i = 0 ; i < line_list.size() - 1 ; i++) {
    uchar *begin = line_list[i];
    uchar *end = line_list[i+1];
    //\0\0\0같이 개행연속으로 붙을떄도 잇어서 완벽하지 않다
    //하지만 이정도만 되도 로직수행에는 문제 없다
    int length = end - begin - 1;
    length_list.push_back(length);
  }
  length_list.push_back(strlen((const char*)line_list.back()));
}

void ObjLoader::ParseObjLine(uchar *str, int n) {
  SR_ASSERT(str != NULL);
  SR_ASSERT(n >= 1);

  string line((char*)str, n);
  // check is comment
  if(line[0] == '#') {
    return;
  }

  //parse line
  typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
  boost::char_separator<char> sep(" \n\r\t", "/");
  tokenizer tokens(line, sep);

  tokenizer::iterator tok_iter = tokens.begin();
  if(tok_iter == tokens.end()) {
    return;
  }

  if(*tok_iter == "v") {
    ++tok_iter; //pass command
    Vec3f pos;
    for(int i = 0 ; tok_iter != tokens.end() ; ++tok_iter, ++i) {
      string str = *tok_iter;
      pos[i] = (float)atof(str.c_str());
    }
    impl->pos_list.push_back(pos);

  } else if(*tok_iter == "vn") {
    ++tok_iter; //pass command
    Vec3f normal;
    for(int i = 0 ; tok_iter != tokens.end() ; ++tok_iter, ++i) {
      string str = *tok_iter;
      normal[i] = (float)atof(str.c_str());
    }
    impl->normal_list.push_back(normal);

  } else if(*tok_iter == "vt") {
    ++tok_iter; //pass command
    Vec2f texcoord;
    for(int i = 0 ; tok_iter != tokens.end() ; ++tok_iter, ++i) {
      string str = *tok_iter;
      texcoord[i] = (float)atof(str.c_str());
    }
    impl->tex_list.push_back(texcoord);

  } else if(*tok_iter == "f") {
    ++tok_iter; //pass command

    vector<int> value_list;
    const int kSeperator = INT_MAX;
    int seperator_count = 0;
    int index_count = 0;
    for( ; tok_iter != tokens.end() ; ++tok_iter) {
      if(*tok_iter == "/") {
        seperator_count++;
      } else {
        string str = *tok_iter;
        value_list.push_back(atof(str.c_str()));
        index_count++;
      }
    }

    //vertex :  f v1 v2 v3 v4 ...
    //Vertex/texture-coordinate : f v1/vt1 v2/vt2 v3/vt3 ...
    //Vertex/texture-coordinate/normal : f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3 ...
    //Vertex/normal : f v1//vn1 v2//vn2 v3//vn3 .
    int face_type = 0;
    if(seperator_count == 0) {
      //vertex
      face_type = kFaceVertex;
    } else {
      if(seperator_count*2 == index_count) {
        //Vertex/texture-coordinate 
        face_type = kFaceVertexTex;
      } else if(index_count == seperator_count) {
        //Vertex/normal 
        face_type = kFaceVertexNormal;
      } else if(seperator_count * 1.5f == index_count) {
        //Vertex/texture-coordinate/normal
        face_type = kFaceVertexTexNormal;
      } else {
        SR_ASSERT(!"not valid face format");
      }
    }

    //face타입에 따라서 적절히 파싱
    int vert_count = GetFaceVertexCount(face_type, value_list.size());
    SR_ASSERT(vert_count == 3 && "삼각형 메시만 일단 취급");
    vector<Vertex> vert_list(vert_count);

    if(face_type == kFaceVertex) {
      for(int i = 0 ; i < vert_count ; i++) {
        int pos_idx = value_list[i];
        vert_list[i].pos = impl->pos_list[pos_idx];
      }

    } else if(face_type == kFaceVertexTex) {
      for(int i = 0 ; i < vert_count ; i++) {
        int pos_idx = value_list[i * 2 + 0];
        int texcoord_idx = value_list[i * 2 + 1];
        vert_list[i].pos = impl->pos_list[pos_idx];
        vert_list[i].texcoord = impl->tex_list[texcoord_idx];
      }

    } else if(face_type == kFaceVertexNormal) {
      for(int i = 0 ; i < vert_count ; i++) {
        int pos_idx = value_list[i * 2 + 0];
        int normal_idx = value_list[i * 2 + 1];
        vert_list[i].pos = impl->pos_list[pos_idx];
        vert_list[i].normal = impl->normal_list[normal_idx];
      }

    } else if(face_type == kFaceVertexTexNormal) {
      for(int i = 0 ; i < vert_count ; i++) {
        int pos_idx = value_list[i * 3 + 0];
        int texcoord_idx = value_list[i * 3 + 1];
        int normal_idx = value_list[i * 3 + 2];
        vert_list[i].pos = impl->pos_list[pos_idx];
        vert_list[i].texcoord = impl->tex_list[texcoord_idx];
        vert_list[i].normal = impl->normal_list[normal_idx];
      }
    }

    int base_vert_size = impl->model->vertex_count();
    for(int i = 0 ; i < 3 ; i++) {
      impl->model->AddVertex(vert_list[i]);
    }
    impl->model->AddIndex(base_vert_size + 0);
    impl->model->AddIndex(base_vert_size + 1);
    impl->model->AddIndex(base_vert_size + 2);


  } else if(*tok_iter == "g") {
    ++tok_iter; //pass command

  } else {
    //unknown..or space
  }
}

int ObjLoader::GetFaceVertexCount(int face_type, int elem_count) {
  if(face_type == kFaceVertex) {
    return elem_count;
  } else if(face_type == kFaceVertexTex) {
    return elem_count / 2;
  } else if(face_type == kFaceVertexNormal) {
    return elem_count / 2;
  } else if(face_type == kFaceVertexTexNormal) {
    return elem_count / 3;
  } else {
    SR_ASSERT(!"not valid face");
    return 0;
  }
}
ObjLoader::ObjLoader()
: impl(new ObjLoaderImpl()) {
  //obj model 포맷에서의 인덱스는 1부터 시작한다. 배열의 인덱스 0에는 딴거를 넣어서
  //추가적인 인덱스 수정없이 바로 쓸수잇도록하자
  impl->pos_list.reserve(128);
  impl->tex_list.reserve(128);
  impl->normal_list.reserve(128);
  //add index 0
  impl->pos_list.push_back(Vec3f(FLT_MAX, FLT_MAX, FLT_MAX));
  impl->tex_list.push_back(Vec2f(FLT_MAX, FLT_MAX));
  impl->normal_list.push_back(Vec3f(FLT_MAX, FLT_MAX, FLT_MAX));
}

ObjLoader::~ObjLoader() {
  delete(impl);
}

void ObjLoader::LoadObj(uchar *start, uchar *end, ObjModel *model) {
  impl->model = model;

  ObjLoader::NewLineToNullChar(start, end);
  vector<uchar*> line_list;
  vector<int> length_list;
  int line_count = ObjLoader::GetLineStartPos(start, end, line_list);
  ObjLoader::GetLineLength(line_list, length_list);

  for(int i = 0 ; i < line_count ; i++) {
    uchar *line = line_list[i];
    int length = length_list[i];
    ParseObjLine(line, length);
  }
}

void ObjLoader::LoadMtl(uchar *start, uchar *end, std::vector<Material> *mtl_list) {
  impl->mtl_list = mtl_list;

  ObjLoader::NewLineToNullChar(start, end);
  vector<uchar*> line_list;
  vector<int> length_list;
  int line_count = ObjLoader::GetLineStartPos(start, end, line_list);
  ObjLoader::GetLineLength(line_list, length_list);

  for(int i = 0 ; i < line_count ; i++) {
    uchar *line = line_list[i];
    int length = length_list[i];
    ParseMtlLine(line, length);
  }
}
void ObjLoader::ParseMtlLine(uchar *str, int n) {
  SR_ASSERT(str != NULL);
  SR_ASSERT(n >= 1);

  string line((char*)str, n);
  // check is comment
  if(line[0] == '#') {
    return;
  }

  //parse line
  typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
  boost::char_separator<char> sep(" \n\r\t", "/");
  tokenizer tokens(line, sep);

  tokenizer::iterator tok_iter = tokens.begin();
  if(tok_iter == tokens.end()) {
    return;
  }

  string cmd = *tok_iter;
  ++tok_iter;
  if(cmd == "newmtl") {
    string mtl_name = *tok_iter;
    Material mtl;
    strcpy(mtl.name, mtl_name.c_str());
    impl->mtl_list->push_back(mtl);

  } else if(cmd == "Ka") {
    Material *mtl = impl->GetLastMtl();
    for(int i = 0 ; i < 3 ; i++) {
      string color_str = *tok_iter;
      ++tok_iter;
      float color = (float)atof(color_str.c_str());
      mtl->ambient[i] = color;
    }

  } else if(cmd == "Kd") {
    Material *mtl = impl->GetLastMtl();
    for(int i = 0 ; i < 3 ; i++) {
      string color_str = *tok_iter;
      ++tok_iter;
      float color = (float)atof(color_str.c_str());
      mtl->diffuse[i] = color;
    }

  } else if(cmd == "Ks") {
    Material *mtl = impl->GetLastMtl();
    for(int i = 0 ; i < 3 ; i++) {
      string color_str = *tok_iter;
      ++tok_iter;
      float color = (float)atof(color_str.c_str());
      mtl->specular[i] = color;
    }

  } else if(cmd == "illum") {
    string illum_str = *tok_iter;
    int illum = atoi(illum_str.c_str());
    Material *mtl = impl->GetLastMtl();
    mtl->illumination_model = illum;

  } else if(cmd == "Ns") {
    string s_str = *tok_iter;
    float s = (float)atof(s_str.c_str());
    Material *mtl = impl->GetLastMtl();
    mtl->shininess = s;

  } else if(cmd == "d") {
    string alpha_str = *tok_iter;
    float alpha = (float)atof(alpha_str.c_str());
    Material *mtl = impl->GetLastMtl();
    mtl->alpha = alpha;

  } else if(cmd == "Tr") {
    string alpha_str = *tok_iter;
    float alpha = (float)atof(alpha_str.c_str());
    Material *mtl = impl->GetLastMtl();
    mtl->alpha = alpha;

  } else if(cmd == "map_Ka") {
    string filename= *tok_iter;
    Material *mtl = impl->GetLastMtl();
    strcpy(mtl->tex_map, filename.c_str());

  } else {
    SR_ASSERT(!"not valid cmd");
  }
  
}
}

