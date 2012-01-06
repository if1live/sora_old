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
#ifndef SORA_HANDLE_MANAGER_H_
#define SORA_HANDLE_MANAGER_H_

#include "template_library.h"
#include "arch.h"
#include "assert_inc.h"
#include <map>

// GPG 1 1.6 범용 핸글 기반 자원 관리자의 코드를 대강 수정한거 
namespace sora {;
template<typename TagType>  class Handle;

template<typename TagType>
class Handle {
public:
  enum {
    kMaxBitsIndex = 16,
    kMaxBitsMagic = 16,
    kMaxIndex = (1 << kMaxBitsIndex) - 1,
    kMaxMagic = (1 << kMaxBitsMagic) - 1,
  };
public:
  Handle() : handle_(0) { }
  Handle(int index) : handle_(0) { Init(index); }
  ~Handle() { }
 
  u32 res_index() const { return res_index_; }
  u32 res_magic() const { return res_magic_; }
  void set_res_index(int i) { res_index_ = i; }
  u32 handle() const { return handle_; }
  bool IsNull() const { return (!handle_); }
  operator u32() const { return handle_; }

  void Init(u32 index) {
    SR_ASSERT(IsNull());    // 이미 사용되는 핸들 고치기 불가능
    SR_ASSERT(index <= kMaxIndex);

    static unsigned int auto_magic = 0; // magic=0 == null handle
    auto_magic++;
    if (auto_magic > kMaxMagic) {
      auto_magic = 1;
    }
    res_index_ = index;
    res_magic_ = auto_magic;
  }

  bool operator!=(const Handle<TagType> &o) const {
    return !(*this == o);
  }
  bool operator==(const Handle<TagType> &o) const {
    return (this->handle() == o.handle());
  }

private:
  union {
    struct {
      unsigned res_index_ : kMaxBitsIndex;  // 자원 벡터에 대한 인덱스
      unsigned res_magic_ : kMaxBitsMagic;  // 오류 확인용
    };
    u32 handle_;
  };
};


template<
  typename DataType,
  typename HandleType
>
class DynamicHandleManager;


template<typename DataType, typename HandleType>
struct OnNullHandlePolicy_Get {
  static DataType *OnNullHandle(HandleType &handle,
    DynamicHandleManager<DataType, HandleType> *mgr) {
    return NULL;
  }
};
template<typename DataType, typename HandleType>
struct OnNullHandlePolicy_CreateOrGet {
  static DataType *OnNullHandle(HandleType &handle,
    DynamicHandleManager<DataType, HandleType> *mgr) {
    typedef DynamicHandleManager<DataType, HandleType> MgrType;
    // 목록에 아무것도 없으면 검색이 불가능하다. 그냥 새로 만들어서
    // 그것의 핸들로 교체후 반환한다
    DataType *data = new DataType();
    handle.Init(mgr->data_list_.size());
    u16 magic = handle.res_magic();
    MgrType::DataPairType data_pair = std::make_pair(magic, data);
    mgr->data_list_.push_back(data_pair);
    return data;
  }
};

template<
  typename DataType,
  typename HandleType
>
class DynamicHandleManager : public Singleton <DynamicHandleManager<DataType, HandleType> > {
private:
  friend struct OnNullHandlePolicy_CreateOrGet<DataType, HandleType>;
  friend struct OnNullHandlePolicy_Get<DataType, HandleType>;
public:
  // 동적할당된 객체와 그것의 magic는 같이 붙어서 움직인다
  typedef std::pair<u16, DataType*> DataPairType;
  typedef std::vector<DataPairType> DataListType;
private:
  DataListType data_list_;    //실제 자원을 넣는것
public:
  DynamicHandleManager() {
  }

  ~DynamicHandleManager() {
    DataListType::iterator it = data_list_.begin();
    DataListType::iterator endit = data_list_.end();
    for ( ; it != endit ; it++) {
      const DataPairType &data_pair = *it;
      DataType *data = data_pair.second;
      delete(data);
    }
  }

  // 핸들에 해당되는것이 존재하지 않으면 NULL
  DataType *Get(HandleType &handle) {
    return GetWithOnNullHandlePolicy<OnNullHandlePolicy_Get<DataType, HandleType> >(handle);
  }

  // 해당되는 핸들이 존재하지 않으면 새로 만들고, 있으면 그거 반환
  // 핸들의 타입을 const로 하지 않는 이유는
  // 내부의 자료구조가 변경되서 handle의 인덱스가 invalid가 된경우, 그것을 다시
  // 올바르게 바꿔주는 기능이 있기떄문
  DataType *CreateOrGet(HandleType &handle) {
    return GetWithOnNullHandlePolicy<OnNullHandlePolicy_CreateOrGet<DataType, HandleType> >(handle);
  }
  void Remove(HandleType &handle) {
    // 핸들을 써서 바로 접근 가능한 상황의 경우. 즉, invalid아닌 경우
    // 해당 인덱스로 접근이 가능하고
    if (handle.res_index() < data_list_.size()) {
      // 올바른 magic이면 객체를 삭제
      const DataPairType &candidate_pair = data_list_[handle.res_index()];
      if (candidate_pair.first == handle.res_magic()) {
        DataListType::iterator it = data_list_.begin();
        std::advance(it, handle.res_index());
        DataType *data = candidate_pair.second;
        delete(data);
        data_list_.erase(it);
        return;
      }
    }

    // 이전에 밀린적이 있다든가 하는 경우는 리스트를 전체에 대해서 같은 magic를 찾아봐야한다
    DataListType::iterator it = data_list_.begin();
    DataListType::iterator endit = data_list_.end();
    for ( ; it != endit ; it++) {
      const DataPairType &candidate_pair = *it;
      if (candidate_pair.first == handle.res_magic()) {
        DataType *data = candidate_pair.second;
        delete(data);
        data_list_.erase(it);
        return;
      }
    }
  }

  int GetHandleCount() const {
    return data_list_.size();
  }

private:
  template<typename OnNullPolicy>
  DataType *GetWithOnNullHandlePolicy(HandleType &handle) {
    // 널 핸들이면 객체를 검색못하니까 만든다
    if (handle.IsNull()) {
      return OnNullPolicy::OnNullHandle(handle, this);
    }

    // 해당 인덱스로 접근이 가능하고
    if (handle.res_index() < data_list_.size()) {
      // handle 인덱스에 있는 객체가 magic이 동일하면 그 객체 맞을거다
      const DataPairType &candidate_pair = data_list_[handle.res_index()];
      if (candidate_pair.first == handle.res_magic()) {
        return candidate_pair.second;
      }
    }

    // 핸들이 invalid발생했을 가능성이 있다. 리소스 목록을 전부 뒤져본다
    // 올바른게 있으면 그것을 반환하고 핸들도 올바르게 고친다
    for (int i = 0 ; i < data_list_.size() ; i++) {
      const DataPairType &candidate_pair = data_list_[i];
      if (candidate_pair.first == handle.res_magic()) {
        handle.set_res_index(i);
        return candidate_pair.second;
      }
    }

    //else...
    return NULL;
  }
};

/*
template<typename DataType, typename HandleType>
class StaticHandleManager {
};
*/
}
#endif  // SORA_HANDLE_MANAGER_H_