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
#ifndef SORA_GL_HELPER_H_
#define SORA_GL_HELPER_H_

#if SR_USE_PCH == 0
#include <vector>
#include <string>
#endif

namespace sora {;
class GLHelper {
public:
  static bool CheckError(const char *name);
  static bool CheckError(const std::string &name) {
    return CheckError(name.c_str());
  }
  // gl information
  static const std::string &GetVersion();
  static const std::string &GetVender();
  static const std::string &GetRenderer();
  static const std::string &GetExtensions();
  static const std::vector<std::string> &GetExtensionList();
  static bool IsExtensionSupport(const char *ext) {
    return IsExtensionSupport(std::string(ext));
  }
  static bool IsExtensionSupport(const std::string &ext);
};

}

#endif  // SORA_GL_HELPER_H_