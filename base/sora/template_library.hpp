// Ŭnicode please
#pragma once

namespace sora
{
template<typename AssocType>
void DestroyDict(AssocType &dict)
{
  typename AssocType::iterator it = dict.begin();
  typename AssocType::iterator endit = dict.end();
  for ( ; it != endit ; it++) {
    delete(it->second);
  }
  dict.clear();
}

template<typename SequenceType>
void DestroyList(SequenceType &list)
{
  typename SequenceType::iterator it = list.begin();
  typename SequenceType::iterator endit = list.end();
  for ( ; it != endit ; it++) {
    delete(*it);
  }
  list.clear();
}
}