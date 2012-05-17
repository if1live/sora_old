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

#include "unordered_map_inc.h"
#include "globals.h"
#include "shader.h"

namespace sora {;
struct Material;

class UberShader {
public:
  UberShader();
  ~UberShader();

  void InitWithFile(const char *vert_file, const char *frag_file, uint avail_mask);
  void Deinit();
  Shader &Load(uint flag);

  bool IsValidFlag(uint flag) const;

public:
  void ApplyCamera();
  void ApplyCamera(const glm::mat4 &proj, const glm::mat4 &view, const glm::mat4 &model);

  void ApplyMaterial();
  void ApplyMaterial(
    const glm::vec4 &light_ambient, 
    const glm::vec4 &light_diffuse, 
    const glm::vec4 &light_specular
    );
  void ApplyMaterial(const Material &material);

protected:
  void LoadRawSrc(const std::string &v_file, const std::string &f_file);

  typedef std::tr1::unordered_map<unsigned int, Shader> ShaderDictType;
  ShaderDictType prog_dict_;

  std::string orig_vert_src_;
  std::string orig_frag_src_;
  uint avail_mask_;
};
} //namespace sora