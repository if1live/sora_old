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

namespace sora {;

struct VertexBufferInterface {
  VertexBufferInterface() {}
  virtual ~VertexBufferInterface() {}

  virtual int size() const = 0;
  virtual bool empty() const = 0;
  virtual void Deinit() = 0;
  virtual bool IsBuffer() const = 0;
  virtual void *ptr() = 0;
  virtual const void *ptr() const = 0;
  virtual int ElemSize() const = 0;
  
  virtual VertexBufferHandle handle() const = 0;
  virtual VertexCode vertex_code() const = 0;
};

class IndexBufferInterface {
public:
  IndexBufferInterface() {}
  virtual ~IndexBufferInterface() {}

  virtual int size() const = 0;
  virtual bool empty() const = 0;
  virtual void Deinit() = 0;
  virtual bool IsBuffer() const = 0;
  virtual void *ptr() = 0;
  virtual const void *ptr() const = 0;

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
  void Init(const std::vector<T> &data) { data_ = data; }
  int size() const { return data_.size(); }
  T *data() { return data_.data(); }
  const T *data() const { return data_.data(); }
  bool empty() const { return data_.empty(); }
  void Deinit() { data_.clear(); }
  bool IsBuffer() const { return false; }
  void *ptr() { return data(); }
  const void *ptr() const { return data(); }
  int ElemSize() const { return sizeof(T); }
  VertexBufferHandle handle() const { return 0; }

  VertexCode vertex_code() const { return (VertexCode)VertexInfoHolder<T>::code; }

protected:
  std::vector<T> data_;
};

template<typename T>
class VertexArray : public VectorWrapper<T, VertexBufferInterface> {
public:
  VertexArray() {}
  VertexArray(const std::vector<T> &data) : VectorWrapper(data) {}
};

}