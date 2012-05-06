/*  Copyright (C) 2011 by if1live */
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
#include <boost/tokenizer.hpp>

using namespace std;

namespace sora {;

enum {
  kFaceVertex,
  kFaceVertexTex,
  kFaceVertexTexNormal,
  kFaceVertexNormal
};

void ObjFormatHelper::NewLineToNullChar(uchar *start, uchar *end) {
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

int ObjFormatHelper::GetLineStartPos(uchar *start, uchar *end, std::vector<uchar*> &line_list) {
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

void ObjFormatHelper::GetLineLength(const std::vector<uchar*> &line_list, std::vector<int> &length_list) {
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

/////////////////////////////////

MtlLoader::MtlLoader() : mtl_list_(NULL) {

}
MtlLoader::~MtlLoader() {

}
void MtlLoader::Load(uchar *start, uchar *end, std::vector<Material> *mtl_list) {
  mtl_list_ = mtl_list;

  ObjFormatHelper::NewLineToNullChar(start, end);
  vector<uchar*> line_list;
  vector<int> length_list;
  int line_count = ObjFormatHelper::GetLineStartPos(start, end, line_list);
  ObjFormatHelper::GetLineLength(line_list, length_list);

  for(int i = 0 ; i < line_count ; i++) {
    uchar *line = line_list[i];
    int length = length_list[i];
    Parse(line, length);
  }
  mtl_list_ = NULL;
}
void MtlLoader::Parse(uchar *str, int n) {
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
    mtl.name = mtl_name;
    mtl_list_->push_back(mtl);

  } else if(cmd == "Ka") {
    Material *mtl = GetLastMtl();
    for(int i = 0 ; i < 3 ; i++) {
      string color_str = *tok_iter;
      ++tok_iter;
      float color = (float)atof(color_str.c_str());
      mtl->ambient[i] = color;
    }
    mtl->props |= kMaterialAmbient;

  } else if(cmd == "Kd") {
    Material *mtl = GetLastMtl();
    for(int i = 0 ; i < 3 ; i++) {
      string color_str = *tok_iter;
      ++tok_iter;
      float color = (float)atof(color_str.c_str());
      mtl->diffuse[i] = color;
    }
    mtl->props |= kMaterialDiffuse;

  } else if(cmd == "Ks") {
    Material *mtl = GetLastMtl();
    for(int i = 0 ; i < 3 ; i++) {
      string color_str = *tok_iter;
      ++tok_iter;
      float color = (float)atof(color_str.c_str());
      mtl->specular[i] = color;
    }
    mtl->props |= kMaterialSpecular;

  } else if(cmd == "illum") {
    /*
    string illum_str = *tok_iter;
    int illum = atoi(illum_str.c_str());
    Material *mtl = impl->GetLastMtl();
    mtl->illumination_model = illum;
    */

  } else if(cmd == "Ns") {
    string s_str = *tok_iter;
    float s = (float)atof(s_str.c_str());
    Material *mtl = GetLastMtl();
    mtl->shininess = s;
    mtl->props |= kMaterialSpecular;

  } else if(cmd == "d") {
    //string alpha_str = *tok_iter;
    //float alpha = (float)atof(alpha_str.c_str());
    //Material *mtl = impl->GetLastMtl();
    //mtl->alpha = alpha;

  } else if(cmd == "Tr") {
    //string alpha_str = *tok_iter;
    //float alpha = (float)atof(alpha_str.c_str());
    //Material *mtl = impl->GetLastMtl();
    //mtl->alpha = alpha;

  } else if(cmd == "map_Ks") {
    string filename= *tok_iter;
    Material *mtl = GetLastMtl();
    mtl->specular_map = filename;
    mtl->props |= kMaterialSpecularMap;

  } else if(cmd == "map_Kd") {
    string filename= *tok_iter;
    Material *mtl = GetLastMtl();
    mtl->diffuse_map = filename;
    mtl->props |= kMaterialDiffuseMap;

  } else {
    ;
  }
  
}
Material *MtlLoader::GetLastMtl() {
  SR_ASSERT(mtl_list_ != NULL);
  SR_ASSERT(mtl_list_->size() > 0);
  return &mtl_list_->back();
}

/////////////////////////////////
ObjLoader::ObjLoader() {

}
ObjLoader::~ObjLoader() {

}
}