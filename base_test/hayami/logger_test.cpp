// Ŭnicode please
#include "precompile.h"
#include "hayami/logger.h"
#include "hayami/log_stream.h"

using namespace hayami;
using namespace std;

TEST(Logger, Log) {
  ConsoleLogger logger(string("sample"));
  logger.set_level(kLogLevelWarning);
  logger.Debug("log debug");
  logger.Notice("log notice");
  logger.Warning("log warning");
  logger.Error("log error");
}

TEST(Logger, Logf) {
  ///가변인자가 들어가는 로거
  ConsoleLogger logger(string("sample"));
  logger.Debugf("log debug %d", 1);
  logger.Noticef("log notice %d", 2);
  logger.Warningf("log warning %d", 3);
  logger.Errorf("log error %d", 4);
}