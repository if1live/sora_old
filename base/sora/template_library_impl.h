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
#ifndef BASE_SORA_TEMPLATE_LIBRARY_IMPL_H_
#define BASE_SORA_TEMPLATE_LIBRARY_IMPL_H_

namespace sora {;
template<typename AssocType>
void DestroyDict(AssocType *dict) {
  typename AssocType::iterator it = dict->begin();
  typename AssocType::iterator endit = dict->end();
  for ( ; it != endit ; it++) {
    delete(it->second);
  }
  dict->clear();
}

template<typename SequenceType>
void DestroyList(SequenceType *list) {
  typename SequenceType::iterator it = list->begin();
  typename SequenceType::iterator endit = list->end();
  for ( ; it != endit ; it++) {
    delete(*it);
  }
  list->clear();
}
}
#endif  // BASE_SORA_TEMPLATE_LIBRARY_IMPL_H_
