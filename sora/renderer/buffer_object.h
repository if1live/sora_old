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

template<typename VertexT> class VertexBufferObjectT;
typedef VertexBufferObjectT<Vertex> VertexBufferObject;
typedef VertexBufferObjectT<Vertex2D> Vertex2DBufferObject;
typedef VertexBufferObjectT<TangentVertex> TangentVertexBufferObject;

class IndexBufferObject;

template<typename VertexT>
class VertexBufferObjectT : public BaseVBOPolicy {
public:
  typedef VertexT VertexType;
  typedef VertexT value_type;
  typedef BaseVBOPolicy::HandleType HandleType;

public:
  VertexBufferObjectT() : size_(0) { BaseVBOPolicy::Reset(&handle_); }
  ~VertexBufferObjectT() {}
  bool Loaded() const { return BaseVBOPolicy::Loaded(handle_); }

  bool Init(const std::vector<VertexType> &vert_list, BufferUsageType usage = kBufferUsageStatic) {
    SR_ASSERT(Loaded() == false);
    if(vert_list.empty()) {
      return false;
    } else {}
      int size = vert_list.size() * sizeof(VertexType);
      BaseVBOPolicy::Init(&handle_, size, (void*)&vert_list[0], usage);
      size_ = vert_list.size();
      return true;
  }
  void Deinit() { BaseVBOPolicy::Deinit(&handle_); }
  int size() const { return size_; }

  VertexType *data() { return nullptr; }
  const VertexType *data() const { return nullptr; }

  HandleType handle() const { return handle_; }
  
private:
  HandleType handle_;
  int size_;
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



class IndexBufferObject : public BaseIBOPolicy {
public:
  typedef BaseIBOPolicy::HandleType HandleType;
public:
  IndexBufferObject() : size_(0) { BaseIBOPolicy::Reset(&handle_); }
  ~IndexBufferObject() {}
  bool Loaded() const { return BaseIBOPolicy::Loaded(handle_); }
  void Deinit() { BaseIBOPolicy::Deinit(&handle_); }

  template<typename IndexContainer>
  bool Init(const IndexContainer &index_list, BufferUsageType usage = kBufferUsageStatic) {
    //typedef IndexContainer::value_type IndexElemType;
    //static_assert(std::is_same<unsigned short, IndexElemType>::value, "not unsigned short index");

    SR_ASSERT(Loaded() == false);
    if(index_list.empty()) {
      return false;
    } else {
      int size = index_list.size() * sizeof(index_list[0]);
      BaseIBOPolicy::Init(&handle_, size, (void*)&index_list[0], usage);
      size_ = index_list.size();
      return true;
    }
  }
  int size() const { return size_; }

  IndexType *data() { return nullptr; }
  const IndexType *data() const { return nullptr; }
  HandleType handle() const { return handle_; }

private:
  HandleType handle_;
  int size_;
};

//인덱스 버퍼에 대한 추가 정보를 가지고 있는 것
template<typename T>
struct IndexBufferInfoHolder {
  typedef IndexBufferObject::HandleType HandleType;

};

template<>
struct IndexBufferInfoHolder< std::vector<unsigned short> > {
  enum {
    is_buffer = false,
  };
  static IndexBufferObject::HandleType buffer(const std::vector<unsigned short> &o) { return 0; }
};

template<>
struct IndexBufferInfoHolder< IndexBufferObject > {
  enum {
    is_buffer = true,
  };
  static IndexBufferObject::HandleType buffer(const IndexBufferObject &o) { return o.handle(); }
};

//버텍스 버퍼에 대한 추가 정보를 가지고 있는것
template<typename T>
struct VertexBufferInfoHolder {
};

template<typename T>
struct VertexBufferInfoHolder< std::vector<T> > {
  enum {
    is_buffer = false,
  };
  static VertexBufferObjectT<Vertex>::HandleType buffer(const std::vector<T> &o) { return 0; }
};

template<typename T>
struct VertexBufferInfoHolder< VertexBufferObjectT<T> > {
  enum {
    is_buffer = true,
  };
  static VertexBufferObjectT<Vertex>::HandleType buffer(const VertexBufferObjectT<T> &o) { return o.handle(); }
};
}

#endif  // SORA_BUFFER_OBJECT_H_