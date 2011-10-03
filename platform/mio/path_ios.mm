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
// Ŭnicode pleasep
#include "mio/path.h"
#include <string>
#include "sora/platform.h"

#if SR_IOS

#include "mio/path.h"
#import <Foundation/Foundation.h>

using std::string;

namespace mio {;
std::string PathCore::docPath(const std::string &file) {
  NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
  NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
  NSString *docDir = [paths objectAtIndex:0];
    
  NSString *nsfilename = [NSString stringWithCString:file.c_str() encoding:NSUTF8StringEncoding];
  NSString *path = [docDir stringByAppendingPathComponent:nsfilename];
  std::string result = [path cStringUsingEncoding:NSUTF8StringEncoding];
  [pool drain];
  return result;
}
std::string PathCore::appPath(const std::string &file) {
  NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
  NSString *filenameStr = [NSString stringWithCString:file.c_str() encoding:NSUTF8StringEncoding];
  NSString *path = [[NSBundle mainBundle] pathForResource:filenameStr ofType:nil];
    
  const char *result = NULL;
  if ([path length] == 0) {
    result = "";
  } else {
    result = [path cStringUsingEncoding:NSUTF8StringEncoding];
  }
  string resultStr(result);
  [pool drain];
  return resultStr;
}
bool PathCore::isFileExist(const std::string &file) {
  NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
  NSString *path = [NSString stringWithCString:file.c_str() encoding:NSUTF8StringEncoding];
  BOOL exist = [[NSFileManager defaultManager] fileExistsAtPath:path];
  [pool drain];
  if (exist == YES) {
    return true;
  } else {
    return false;
  }
}
}
#endif
