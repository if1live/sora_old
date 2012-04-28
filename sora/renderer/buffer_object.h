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

#include "renderer/globals.h"
#include "renderer/gl/gl_buffer_object.h"

namespace sora {;
template<typename VertexT, typename GLVBOPolicy> class VertexBufferObjectT;
typedef VertexBufferObjectT<Vertex, GLVBOPolicy> VertexBufferObject;
typedef VertexBufferObjectT<Vertex2D, GLVBOPolicy> Vertex2DBufferObject;
typedef VertexBufferObjectT<TangentVertex, GLVBOPolicy> TangentVertexBufferObject;

template<typename BasePolicy> class IndexBufferObjectT;
typedef IndexBufferObjectT<GLIBOPolicy> IndexBufferObject;

template<typename VertexT, typename BasePolicy>
class VertexBufferObjectT : public BasePolicy {
public:
  typedef VertexT VertexType;
  typedef VertexT value_type;
  typedef VertexBufferHandle HandleType;

public:
  VertexBufferObjectT() : size_(0) { BasePolicy::Reset(&handle_); }
  ~VertexBufferObjectT() {}
  bool Loaded() const { return BasePolicy::Loaded(handle_); }

  bool Init(const std::vector<VertexType> &vert_list, BufferUsageType usage = kBufferUsageStatic) {
    SR_ASSERT(Loaded() == false);
    if(vert_list.empty()) {
      return false;
    } else {}
      int size = vert_list.size() * sizeof(VertexType);
      BasePolicy::Init(&handle_, size, (void*)&vert_list[0], usage);
      size_ = vert_list.size();
      return true;
  }
  void Deinit() { BasePolicy::Deinit(&handle_); }
  int size() const { return size_; }

  VertexType *data() { return nullptr; }
  const VertexType *data() const { return nullptr; }
  bool empty() const { return (size_ == 0); }

  HandleType handle() const { return handle_; }
  
private:
  HandleType handle_;
  int size_;
};

template<typename BasePolicy>
class IndexBufferObjectT {
public:
  typedef IndexBufferHandle HandleType;
public:
  IndexBufferObjectT() : size_(0) { BasePolicy::Reset(&handle_); }
  ~IndexBufferObjectT() {}
  bool Loaded() const { return BasePolicy::Loaded(handle_); }
  void Deinit() { BasePolicy::Deinit(&handle_); }

  template<typename IndexContainer>
  bool Init(const IndexContainer &index_list, BufferUsageType usage = kBufferUsageStatic) {
    //typedef IndexContainer::value_type IndexElemType;
    //static_assert(std::is_same<unsigned short, IndexElemType>::value, "not unsigned short index");

    SR_ASSERT(Loaded() == false);
    if(index_list.empty()) {
      return false;
    } else {
      int size = index_list.size() * sizeof(index_list[0]);
      BasePolicy::Init(&handle_, size, (void*)&index_list[0], usage);
      size_ = index_list.size();
      return true;
    }
  }
  int size() const { return size_; }
  bool empty() const { return (size_ == 0); }

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
  enum {
    is_buffer = true,
  };
  static IndexBufferHandle buffer(const T &o) { return o.handle(); }
};

template<>
struct IndexBufferInfoHolder< std::vector<unsigned short> > {
  enum {
    is_buffer = false,
  };
  static IndexBufferHandle buffer(const std::vector<unsigned short> &o) { return 0; }
};

//버텍스 버퍼에 대한 추가 정보를 가지고 있는것
template<typename T>
struct VertexBufferInfoHolder {
  enum {
    is_buffer = true,
  };
  static VertexBufferHandle buffer(const T &o) { return o.handle(); }
};

template<typename T>
struct VertexBufferInfoHolder< std::vector<T> > {
  enum {
    is_buffer = false,
  };
  static VertexBufferHandle buffer(const std::vector<T> &o) { return 0; }
};

}

#endif  // SORA_BUFFER_OBJECT_H_