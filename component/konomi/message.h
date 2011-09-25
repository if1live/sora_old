// Ŭnicode please
#pragma once

#include "konomi/konomi_enum.h"
#include "konomi/variant.h"

namespace konomi
{
  template<unsigned int N>  class Message;
  typedef Message<4> Message4;
  typedef Message<8> Message8;

  //메세지 객체는 최적화를 초점으로 맞춰서
  //고정크기 객체로 만들고 무식하게 다룰수 있도록 하기 위해서
  //메세지에 들어가는 인자의 갯수를 8개로 제한했다
  //설마 이 이상의 인자가 들어갈 경우는 확장팩을 쓸것
  template<unsigned int N>
  class Message {
  public:
    enum {
      MaxVariantNumber = N
    };
  public:
    Message();
    Message(MessageCode code);
    template<unsigned int N2>
    Message(const Message<N2> &o);
    ~Message();

    MessageCode get_code() const;

    int get_num_param() const;
    void AddParam(const Variant &param);

    template<int index>
    const Variant& GetParam() const;
    const Variant& GetParam(int index) const;

    bool CanAddParam() const;

    template<int index>
    void SetParam(const Variant &param);
    void SetParam(int index, const Variant &param);

    ///@brief 해당 메세지가 몇개의 인자를 사용할지를 
    ///한방에 확정할떄 쓰는 함수
    template<int num>
    void SetNumParam();
    void SetNumParam(int num);

    template<unsigned int N2>
    bool operator==(const Message<N2> &o) const;
    template<unsigned int N2>
    bool operator!=(const Message<N2> &o) const;

    void Clear();
    void ClearParam();
  private:
    MessageCode code_;

    int num_param_;
    Variant param_list_[N];
  };
}

#include "konomi/message.hpp"