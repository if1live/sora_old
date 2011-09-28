// Ŭnicode please
#pragma once

namespace hayami {;
//위에 있는 로그 레벨일수록 높은 수준
typedef enum {
  kLogLevelDebug,
  kLogLevelNotice,
  kLogLevelWarning,
  kLogLevelError,
} LogLevel;

//LogStream 계열
///@brief 로그를 어떤방식으로 보여줄것인가?
///내용을 상속받아 잘 구현한후에 로거에 꽂으면
///로거가 메세지를 콘솔에 찍을수도 있고 파일에 쓸수도 있다
///또한 로거에 따라서 파일 포맷도 어느정도 정할수있다
///스트림 클래스는 void WriteLog(logger_name, msg)를 구현하면 된다
///몰아서 하거나, 에러시 강제를 하기위해서서
///void Flush()도 구현할것
class ConsoleLogStream;

template<typename LogStream>  class Logger;
typedef Logger<ConsoleLogStream> ConsoleLogger;
}