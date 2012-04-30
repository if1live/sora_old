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

#include "vertex.h"
#include "assert_inc.h"

#include "globals.h"
#include "gl_buffer_object.h"

#include "mesh_buffer.h"

namespace sora {;
template<typename VertexT, typename GLVBOPolicy> class VertexBufferObjectT;
typedef VertexBufferObjectT<Vertex, GLVBOPolicy> VertexBufferObject;
typedef VertexBufferObjectT<Vertex2D, GLVBOPolicy> Vertex2DBufferObject;
typedef VertexBufferObjectT<TangentVertex, GLVBOPolicy> TangentVertexBufferObject;

template<typename BasePolicy> class IndexBufferObjectT;
typedef IndexBufferObjectT<GLIBOPolicy> IndexBufferObject;

template<typename VertexT, typename BasePolicy>
class VertexBufferObjectT : public BasePolicy, public VertexBufferInterface {
public:
  typedef VertexT VertexType;
  typedef VertexT value_type;
  typedef VertexBufferHandle HandleType;

public:
  VertexBufferObjectT() : size_(0) { BasePolicy::Reset(&handle_); }
  VertexBufferObjectT(const std::vector<VertexType> &vert_list, BufferUsageType usage = kBufferUsageStatic) 
    : size_(0) { 
      BasePolicy::Reset(&handle_);
      Init(vert_list);
  }

  ~VertexBufferObjectT() {}
  bool Loaded() const { return BasePolicy::Loaded(handle_); }

  bool Init(const std::vector<Vertex> &vert_list) { return InitWithTypeCheck(vert_list);  }
  bool Init(const std::vector<Vertex2D> &vert_list) { return InitWithTypeCheck(vert_list); }
  bool Init(const std::vector<TangentVertex> &vert_list) { return InitWithTypeCheck(vert_list); }
  bool Init(const std::vector<glm::vec2> &vert_list) { return InitWithTypeCheck(vert_list); }
  bool Init(const std::vector<glm::vec3> &vert_list) { return InitWithTypeCheck(vert_list); }

  bool Load(const std::vector<Vertex> &vert_list) { return LoadWithTypeCheck(vert_list);  }
  bool Load(const std::vector<Vertex2D> &vert_list) { return LoadWithTypeCheck(vert_list); }
  bool Load(const std::vector<TangentVertex> &vert_list) { return LoadWithTypeCheck(vert_list); }
  bool Load(const std::vector<glm::vec2> &vert_list) { return LoadWithTypeCheck(vert_list); }
  bool Load(const std::vector<glm::vec3> &vert_list) { return LoadWithTypeCheck(vert_list); }
  bool Load(const std::vector<unsigned short> &index_list) { return LoadWithTypeCheck(vert_list); }

  template<typename T2>
  bool InitWithTypeCheck(const std::vector<T2> &vert_list) {
    SR_ASSERT(Loaded() == false);
    if(vert_list.empty()) {
      return false;
    }
    BasePolicy::Init(&handle_);
    return LoadWithTypeCheck(vert_list);
  }

  template<typename T2>
  bool LoadWithTypeCheck(const std::vector<T2> &vert_list) {
    SR_ASSERT(Loaded() == true);
    if(vert_list.empty()) {
      return false;
    }
    if(std::is_same<T2, VertexType>::value == false) {
      return false;
    }

    BufferUsageType usage = kBufferUsageStatic; //일단은 내장함. 나중에 분리하자
    int size = vert_list.size() * sizeof(VertexType);
    BasePolicy::Load(handle_, size, (void*)&vert_list[0], usage);
    size_ = vert_list.size();
    return true;
  }

  void Deinit() { BasePolicy::Deinit(&handle_); }
  int size() const { return size_; }

  void *data() { return nullptr; }
  const void *data() const { return nullptr; }
  bool empty() const { return (size_ == 0); }

  HandleType handle() const { return handle_; }
  bool IsBuffer() const { return true; }
  int ElemSize() const { return sizeof(VertexType); }
  VertexCode vertex_code() const { return (VertexCode)VertexInfoHolder<VertexType>::code; }

private:
  HandleType handle_;
  int size_;
};

template<typename BasePolicy>
class IndexBufferObjectT : public IndexBufferInterface {
public:
  typedef IndexBufferHandle HandleType;
public:
  IndexBufferObjectT() : size_(0) { BasePolicy::Reset(&handle_); }
  template<typename IndexContainer>
  IndexBufferObjectT(const IndexContainer &index_list, BufferUsageType usage = kBufferUsageStatic)
    : size_(0) {
    BasePolicy::Reset(&handle_);
    Init(index_list);
  }
  ~IndexBufferObjectT() {}
  bool Loaded() const { return BasePolicy::Loaded(handle_); }
  void Deinit() { BasePolicy::Deinit(&handle_); }

  bool Load(const std::vector<unsigned short> &index_list) { 
    SR_ASSERT(Loaded() == true);
    if(index_list.empty()) {
      return false;
    }
    BufferUsageType usage = kBufferUsageStatic;
    int size = index_list.size() * sizeof(index_list[0]);
    BasePolicy::Load(handle_, size, (void*)&index_list[0], usage);
    size_ = index_list.size();
    return true;
  }

  bool Init(const std::vector<unsigned short> &index_list) {
    //typedef IndexContainer::value_type IndexElemType;
    //static_assert(std::is_same<unsigned short, IndexElemType>::value, "not unsigned short index");

    SR_ASSERT(Loaded() == false);
    if(index_list.empty()) {
      return false;
    }
    BasePolicy::Init(&handle_);
    return Load(index_list);
  }

  int size() const { return size_; }
  bool empty() const { return (size_ == 0); }

  void *data() { return nullptr; }
  const void *data() const { return nullptr; }
  HandleType handle() const { return handle_; }
  bool IsBuffer() const { return true; }

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