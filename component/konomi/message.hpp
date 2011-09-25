// Ŭnicode please
namespace konomi
{
  template<unsigned int N>
  Message<N>::Message()
    : code_(0), num_param_(0)
  {
  }
  template<unsigned int N>
  Message<N>::Message(MessageCode code)
    : code_(code), num_param_(0)
  {
  }

  template<unsigned int N>
  template<unsigned int N2>
  Message<N>::Message(const Message<N2> &o)
    : code_(o.get_code()), num_param_(0)
  {
    SR_ASSERT(o.get_num_param() <= N);
    for(int i = 0 ; i < o.get_num_param() ; i++)
    {
      AddParam(o.GetParam(i));
    }
    num_param_ = o.get_num_param();
  }

  template<unsigned int N>
  Message<N>::~Message()
  {
  }

  template<unsigned int N>
  MessageCode Message<N>::get_code() const
  {
    return code_;
  }

  template<unsigned int N>
  int Message<N>::get_num_param() const
  {
    return num_param_;
  }

  template<unsigned int N>
  void Message<N>::AddParam(const Variant &param)
  {
    SR_ASSERT(num_param_ < MaxVariantNumber);
    param_list_[num_param_] = param;
    num_param_++;
  }

  template<unsigned int N>
  const Variant& Message<N>::GetParam(int index) const
  {
    SR_ASSERT(index >= 0 && index < N);	//배열 범위만 체그
    return param_list_[index];	//범위벗어난것은 널객체가 반환될것이다
  }
  template<unsigned int N>
  template<int index>
  const Variant& Message<N>::GetParam() const
  {
    BOOST_STATIC_ASSERT(index >= 0 && index < N);
    return GetParam(index);
  }

  template<unsigned int N>
  template<unsigned int N2>
  bool Message<N>::operator==(const Message<N2> &o) const
  {
    if(get_code() != o.get_code()) {
      return false;
    }
    if(get_num_param() != o.get_num_param()) {
      return false;
    }

    for(int i = 0 ; i < get_num_param() ; i++) {
      if(GetParam(i) != o.GetParam(i)) {
        return false;
      }
    }
    return true;
  }

  template<unsigned int N>
  template<unsigned int N2>
  bool Message<N>::operator!=(const Message<N2> &o) const
  {
    return !(*this == o);
  }

  template<unsigned int N>
  void Message<N>::Clear()
  {
    num_param_ = 0;
    code_ = 0;
  }
  template<unsigned int N>
  void Message<N>::ClearParam()
  {
    num_param_ = 0;
  }

  template<unsigned int N>
  bool Message<N>::CanAddParam() const
  {
    return (N > num_param_);
  }

  template<unsigned int N>
  template<int num>
  void Message<N>::SetNumParam()
  {
    return SetNumParam(num);
  }

  template<unsigned int N>
  void Message<N>::SetNumParam(int num)
  {
    for(int i = 0 ; i < N ; i++) {
      Variant &param = param_list_[i];
      if(i < num) {
        if(param.IsNull()) {
          //int + 0을 기본값으로 적절히 설정한다
          param = (int)(0);
        }
      } else {
        param.SetNull();
      }
    }
    num_param_ = num;
  }

  template<unsigned int N>
  template<int index>
  void Message<N>::SetParam(const Variant &param)
  {
    BOOST_STATIC_ASSERT(index >= 0 && index < N);
    SetParam(index, param);
  }
  template<unsigned int N>
  void Message<N>::SetParam(int index, const Variant &param)
  {
    SR_ASSERT(index >= 0 && index < num_param_);
    param_list_[index] = param;
  }
}