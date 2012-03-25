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
#ifndef SORA_MESH_MANAGER_H_
#define SORA_MESH_MANAGER_H_

#include "core/template_lib.h"
#include "mesh.h"

#if SR_USE_PCH == 0
#include "core/unordered_map_inc.h"
#endif

namespace sora {;

struct ISurface;  //for parametric surface

class MeshManager : public Singleton<MeshManager> {
public:
  MeshManager();
  ~MeshManager();

  bool AddSurface_line(const ISurface &surface, const char *name);
  bool AddSurface_triangle(const ISurface &surface, const char *name);
  bool Add(const std::vector<DrawCommand> &cmd_list, const char *name);

  MeshBufferObject *Get(const char *name);
  bool IsExist(const char *name) const;

private:
  typedef std::tr1::unordered_map<std::string, MeshBufferObject> MeshDictType;
  MeshDictType mesh_dict_;
};
}

#endif  // SORA_MESH_MANAGER_H_