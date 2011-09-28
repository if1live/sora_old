// Ŭnicode please
//#include "hayami/logger.h"

namespace hayami {;
template<typename LogStream>
Logger<LogStream>::Logger(const std::string &name) 
  : name_(name), level_(kLogLevelDebug) {

}
template<typename LogStream>
Logger<LogStream>::~Logger() {

}
template<typename LogStream>
const std::string &Logger<LogStream>::name() const {
  return name_;
}
template<typename LogStream>
void Logger<LogStream>::WriteLog(LogLevel log_level, const std::string &msg) {
  if (log_level >= level_) {
    stream_.WriteLog(name_, msg);
  }
}

template<typename LogStream>
void Logger<LogStream>::Flush() {
  stream_.Flush();
}
}
