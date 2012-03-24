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
#include "sora_stdafx.h"
#include "zip_stream_file.h"

#if SR_ANDROID

#include "sora/core/logger.h"

namespace sora {;

void ZipStreamFile::SetApkFile(const char *apk_file) {
  LOGI("Loadind APK %s", apk_file);
  apk_archive_ = zip_open(apk_file, 0, NULL);
  if (apk_archive_ == NULL) {
    LOGE("Error loading APK");
    return;
  }
  /*
  //Just for debug, print APK contents
  int numFiles = zip_get_num_files(apk_archive_);
  for (int i=0; i<numFiles; i++) {
    const char* name = zip_get_name(apk_archive_, i, 0);
    if (name == NULL) {
      LOGE("Error reading zip file name at index %i : %s", zip_strerror(apk_archive_));
      return;
    }
    LOGI("File %i : %s\n", i, name);
  }
  */
}
void ZipStreamFile::SetApkFile(const std::string &apk_file) {
  SetApkFile(apk_file.c_str());
}

zip* ZipStreamFile::apk_archive_ = NULL;

ZipStreamFile::ZipStreamFile(const char *filepath)
  : filepath_(filepath), file_(NULL) {
  SR_ASSERT(apk_archive_ != NULL);
}

ZipStreamFile::ZipStreamFile(const std::string &filepath)
  : filepath_(filepath), file_(NULL) {
  SR_ASSERT(apk_archive_ != NULL);
}

ZipStreamFile::~ZipStreamFile() {
  Close();
}

bool ZipStreamFile::Open() {
  SR_ASSERT(IsOpened() == false);
  file_ = zip_fopen(apk_archive_, filepath_.c_str(), 0);
  if (!file_) {
    LOGE("Error opening %s from APK", filepath_.c_str());
    return false;
  }
  return true;
}

void ZipStreamFile::Close() {
  if (file_ != NULL) {
    zip_fclose(file_);
    file_ = NULL;
  }
}

int ZipStreamFile::Read(void *buf, int size) {
  return zip_fread(file_, buf, size);
}

int ZipStreamFile::GetLength() const {
  struct zip_stat stat;
  int result = zip_stat(apk_archive_, filepath_.c_str(), 0, &stat);
  if(result == -1) {
    LOGE("Error read stat %s", filepath_.c_str());
  }
  SR_ASSERT(stat.size > 0);
  return stat.size;
}

}
#endif  // SR_ANDROID
