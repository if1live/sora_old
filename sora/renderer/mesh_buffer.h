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

#include "globals.h"
#include "core/vertex.h"

namespace sora {;

struct VertexBufferInterface {
  VertexBufferInterface() {}
  virtual ~VertexBufferInterface() {}

  virtual int size() const = 0;
  virtual bool empty() const = 0;
  virtual void Deinit() = 0;  
  virtual bool IsBuffer() const = 0;
  virtual void *data() = 0;
  virtual const void *data() const = 0;
  virtual int ElemSize() const = 0;
  
  virtual VertexBufferHandle handle() const = 0;
  virtual VertexCode vertex_code() const = 0;

  //template+virtual조합은 불가능하기 떄문에 가능한 vertex 마다 만들어야한다
  virtual bool Init(const std::vector<Vertex> &vert_list) = 0;
  virtual bool Init(const std::vector<Vertex2D> &vert_list) = 0;
  virtual bool Init(const std::vector<TangentVertex> &vert_list) = 0;
  virtual bool Init(const std::vector<glm::vec2> &vert_list) = 0;
  virtual bool Init(const std::vector<glm::vec3> &vert_list) = 0;
};

class IndexBufferInterface {
public:
  IndexBufferInterface() {}
  virtual ~IndexBufferInterface() {}

  virtual int size() const = 0;
  virtual bool empty() const = 0;
  virtual bool Init(const std::vector<unsigned short> &index_list) = 0;
  virtual void Deinit() = 0;
  virtual bool IsBuffer() const = 0;
  virtual void *data() = 0;
  virtual const void *data() const = 0;

  virtual IndexBufferHandle handle() const = 0;
};

//std::vector<Vertex> 와 같은것에 대응하기 위한 용도로 만든 클래스이다
//특정 인터페이스를 상속받아야지만 map같은데 넣을수 있으니까. 
//그래서 기본 인터페이스는 std::vector와 동일하게 설정햇다
template<typename T, typename InterfaceT> class VectorWrapper;
typedef VectorWrapper<unsigned short, IndexBufferInterface> IndexArray;

template<typename T, typename InterfaceT>
class VectorWrapper : public InterfaceT {
public:
  VectorWrapper() {}
  VectorWrapper(const std::vector<T> &data) : data_(data) {}
  ~VectorWrapper() {}

  void clear() { data_.clear(); }
  void push_back(const T &v) { data_.push_back(v); }
  int size() const { return data_.size(); }
  void *data() { return data_.data(); }
  const void *data() const { return data_.data(); }
  bool empty() const { return data_.empty(); }
  void Deinit() { data_.clear(); }
  bool IsBuffer() const { return false; }
  int ElemSize() const { return sizeof(T); }
  VertexBufferHandle handle() const { return 0; }

  VertexCode vertex_code() const { return (VertexCode)VertexInfoHolder<T>::code; }

  //bool Init(const std::vector<T> &data) { data_ = data; return true;}

  bool Init(const std::vector<Vertex> &vert_list) {
    return InitWithTypeCheck(vert_list);
  }
  bool Init(const std::vector<Vertex2D> &vert_list) {
    return InitWithTypeCheck(vert_list);
  }
  bool Init(const std::vector<TangentVertex> &vert_list) {
    return InitWithTypeCheck(vert_list);
  }
  bool Init(const std::vector<glm::vec2> &vert_list) {
    return InitWithTypeCheck(vert_list);
  }
  bool Init(const std::vector<glm::vec3> &vert_list) {
    return InitWithTypeCheck(vert_list);
  }
  bool Init(const std::vector<unsigned short> &index_list) {
    return InitWithTypeCheck(index_list);
  }

  template<typename T2>
  bool InitWithTypeCheck(const std::vector<T2> &vert_list) {
    if(std::is_same<T2, T>::value) {
      data_.resize(vert_list.size());
      memcpy(data_.data(), vert_list.data(), sizeof(T) * data_.size());
      return true;
    } else {
      return false;
    }
  }

protected:
  std::vector<T> data_;
};

template<typename T>
class VertexArrayT : public VectorWrapper<T, VertexBufferInterface> {
public:
  VertexArrayT() {}
  VertexArrayT(const std::vector<T> &data) : VectorWrapper(data) {}
};

}