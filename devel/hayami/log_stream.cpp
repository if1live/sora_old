// Ŭnicode please
#include "hayami/log_stream.h"
#include <cstdlib>

using ::std::string;

namespace hayami {;
ConsoleLogStream::ConsoleLogStream() {
}
ConsoleLogStream::~ConsoleLogStream() {
}
void ConsoleLogStream::WriteLog(const std::string &logger_name,
  const std::string &msg) {
    printf("%s\n", msg.c_str());
}
void ConsoleLogStream::Flush() {
  fflush(stdout);
}
}