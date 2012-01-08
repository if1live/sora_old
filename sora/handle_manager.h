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
//#include "arch.h"
//#include "assert_inc.h"
//#include <map>

// GPG 1 1.6 범용 핸글 기반 자원 관리자의 코드를 대강 수정한거 
namespace sora {;

template<
  typename DataType,
  typename HandleType
>
class DynamicHandleManager;

template<typename DataType>
struct DataTuple {
  u16 magic;
  DataType data;
};

template<
  typename DataType,
  typename HandleType
>
class DynamicHandleManager : public Singleton <DynamicHandleManager<DataType, HandleType> > {
public:
  // 동적할당된 객체와 그것의 magic는 같이 붙어서 움직인다
  typedef DataTuple<DataType*> DataPairType;
  typedef std::vector<DataPairType> DataListType;
  typedef std::vector<int> FreeSlotListType;
private:
  DataListType data_list_;    //실제 자원을 넣는것
  FreeSlotListType free_list_;
public:
  DynamicHandleManager() { }
  ~DynamicHandleManager();

  // 핸들에 해당되는것이 존재하지 않으면 NULL
  DataType *Get(const HandleType &handle);

  // 해당되는 핸들이 존재하지 않으면 새로 만들고, 있으면 그거 반환
  // 핸들의 타입을 const로 하지 않는 이유는
  // 내부의 자료구조가 변경되서 handle의 인덱스가 invalid가 된경우, 그것을 다시
  // 올바르게 바꿔주는 기능이 있기떄문
  DataType *Acquire(HandleType &handle);
  DataType *Create(HandleType &handle);
  bool Remove(HandleType &handle);

  int GetUsedHandleCount() const { return data_list_.size() - free_list_.size(); }
  int GetUnusedHandleCount() const { return free_list_.size(); }

private:
  template<typename OnNullPolicy>
  DataType *GetWithOnNullHandlePolicy(HandleType &handle);
};

/*
template<typename DataType, typename HandleType>
class StaticHandleManager {
};
*/
}

// 워나 위의 구현체가 길어서 인터페이스가 안보인다
// 그래서 분리
namespace sora {;

template<typename DataType, typename HandleType>
DataType *DynamicHandleManager<DataType, HandleType>::Create(HandleType &handle) {
  SR_ASSERT(handle.IsNull() == true);
  if (free_list_.empty()) {
    DataType *data = new DataType();
    int index = data_list_.size();
    handle.Init(index);
    u16 magic = handle.res_magic();
    DataPairType data_pair = { magic, data };
    data_list_.push_back(data_pair);
    return data;

  } else {
    int index = free_list_.back();
    free_list_.pop_back();
    DataType *data = new DataType();
    handle.Init(index);
    DataPairType &data_pair = data_list_[index];
    data_pair.magic = handle.res_magic();
    data_pair.data = data;
    return data;
  }
}


template<typename DataType, typename HandleType>
DynamicHandleManager<DataType, HandleType>::~DynamicHandleManager() {
  DataListType::iterator it = data_list_.begin();
  DataListType::iterator endit = data_list_.end();
  for ( ; it != endit ; it++) {
    const DataPairType &data_pair = *it;
    DataType *data = data_pair.data;
    delete(data);
  }
}

// 핸들에 해당되는것이 존재하지 않으면 NULL
template<typename DataType, typename HandleType>
DataType *DynamicHandleManager<DataType, HandleType>::Get(const HandleType &handle) {
  // 널 핸들이면 못 얻음
  if (handle.IsNull()) {
    return NULL;
  }

  // 해당 인덱스로 접근이 가능하고
  if (handle.res_index() < data_list_.size()) {
    // handle 인덱스에 있는 객체가 magic이 동일하면 그 객체 맞을거다
    const DataPairType &candidate_pair = data_list_[handle.res_index()];
    if (candidate_pair.magic == handle.res_magic()) {
      return candidate_pair.data;
    }
  }
  //else..
  return NULL;
}

// 해당되는 핸들이 존재하지 않으면 새로 만들고, 있으면 그거 반환
// 핸들의 타입을 const로 하지 않는 이유는
// 내부의 자료구조가 변경되서 handle의 인덱스가 invalid가 된경우, 그것을 다시
// 올바르게 바꿔주는 기능이 있기떄문
template<typename DataType, typename HandleType>
DataType *DynamicHandleManager<DataType, HandleType>::Acquire(HandleType &handle) {
  // 널 핸들이면 객체를 검색못하니까 만든다
  if (handle.IsNull()) {
    return Create(handle);
  }

  // 해당 인덱스로 접근이 가능하고
  if (handle.res_index() < data_list_.size()) {
    // handle 인덱스에 있는 객체가 magic이 동일하면 그 객체 맞을거다
    const DataPairType &candidate_pair = data_list_[handle.res_index()];
    if (candidate_pair.magic == handle.res_magic()) {
      return candidate_pair.data;
    }
  }
  //else..
  return NULL;
}

template<typename DataType, typename HandleType>
bool DynamicHandleManager<DataType, HandleType>::Remove(HandleType &handle) {
  // 핸들을 써서 바로 접근 가능한 상황의 경우. 즉, invalid아닌 경우
  // 해당 인덱스로 접근이 가능하고
  if (handle.res_index() < data_list_.size()) {
    // 올바른 magic이면 객체를 삭제
    DataPairType &candidate_pair = data_list_[handle.res_index()];
    if (candidate_pair.magic == handle.res_magic()) {
      // delete prev data + reset
      DataType *data = candidate_pair.data;
      delete(data);
      candidate_pair.data = NULL;
      candidate_pair.magic = 0;

      int index = handle.res_index();
      free_list_.push_back(index);

      return true;
    }
  }

  return false;
}

}

#endif  // SORA_HANDLE_MANAGER_H_