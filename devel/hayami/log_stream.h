// Ŭnicode please
#pragma once

#include <string>

namespace hayami {;
class ConsoleLogStream {
  public:
    ConsoleLogStream();
    ~ConsoleLogStream();

    void WriteLog(const std::string &logger_name, const std::string &msg);
    void Flush();
  };
}
