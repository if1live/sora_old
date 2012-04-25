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
#ifndef SORA_BUFFER_OBJECT_H_
#define SORA_BUFFER_OBJECT_H_

#include "core/vertex.h"
#include "core/assert_inc.h"

#include "renderer/gl/gl_buffer_object.h"

namespace sora {;
typedef sora::gl::GLVertexBufferObject BaseVBOPolicy;
typedef sora::gl::GLIndexBufferObject BaseIBOPolicy;

template<typename T, typename VertexT> class VertexBufferObjectT;
typedef VertexBufferObjectT<BaseVBOPolicy, Vertex> VertexBufferObject;
typedef VertexBufferObjectT<BaseVBOPolicy, Vertex2D> Vertex2DBufferObject;
typedef VertexBufferObjectT<BaseVBOPolicy, TangentVertex> TangentVertexBufferObject;

template<typename T> class IndexBufferObjectT;
typedef IndexBufferObjectT<BaseIBOPolicy> IndexBufferObject;

template<typename T, typename VertexT>
class VertexBufferObjectT {
public:
  typedef VertexT VertexType;
public:
  VertexBufferObjectT() : count_(0) {}
  ~VertexBufferObjectT() {}
  bool Loaded() const { return vbo_.Loaded(); }

  bool Init(const std::vector<VertexType> &vert_list, BufferUsageType usage = kBufferUsageStatic) {
    SR_ASSERT(Loaded() == false);
    if(vert_list.empty()) {
      return false;
    } else {}
      int size = vert_list.size() * sizeof(VertexType);
      vbo_.Init(size, (void*)&vert_list[0], usage);
      count_ = vert_list.size();
      return true;
  }
  void Deinit() { vbo_.Deinit(); }
  T &vbo() { return vbo_; }
  const T &vbo() const { return vbo_; }
  VertexType vert_type() const { return VertexType::Type(); }
  int count() const { return count_; }
private:
  T vbo_;
  int count_;
};

template<typename VertexT>
struct VBOSelector {
};
template<>
struct VBOSelector<sora::Vertex> {
  typedef VertexBufferObject Result;
};
template<>
struct VBOSelector<sora::Vertex2D> {
  typedef Vertex2DBufferObject Result;
};
template<>
struct VBOSelector<sora::TangentVertex> {
  typedef TangentVertexBufferObject Result;
};



template<typename T> 
class IndexBufferObjectT {
public:
  IndexBufferObjectT() : count_(0) {}
  ~IndexBufferObjectT() {}
  bool Loaded() const { return ibo_.Loaded(); }
  void Deinit() { ibo_.Deinit(); }

  template<typename IndexContainer>
  bool Init(const IndexContainer &index_list, BufferUsageType usage = kBufferUsageStatic) {
    //typedef IndexContainer::value_type IndexElemType;
    //static_assert(std::is_same<unsigned short, IndexElemType>::value, "not unsigned short index");

    SR_ASSERT(Loaded() == false);
    if(index_list.empty()) {
      return false;
    } else {
      int size = index_list.size() * sizeof(index_list[0]);
      ibo_.Init(size, (void*)&index_list[0], usage);
      count_ = index_list.size();
      return true;
    }
  }
  int count() const { return count_; }
  T &ibo() { return ibo_; }
  const T &ibo() const { return ibo_; }
private:
  T ibo_;
  int count_;
};
}
#endif  // SORA_BUFFER_OBJECT_H_