#include "sora/string_util.h"

#include <sstream>

namespace sora
{
std::string StringUtil::Trim(const std::string &str)
{
  using namespace std;

  //do no use boost to compile speed + dependenty
  const char *whitespace = "\t\n\x0b\x0c\r ";

  int leftFound = str.find_first_not_of(whitespace);
  //_Tstring left(n == _Tstring::npos ? tmp : tmp.substr(n, tmp.length()));
  int rightFound = str.find_last_not_of(whitespace);
  //_Tstring right(n == _Tstring::npos ? tmp : tmp.substr(0, n + 1));

  if (leftFound == string::npos) {
    leftFound = 0;
  }
  if (rightFound == string::npos) {
    rightFound = str.length();
  }
  string result = str.substr(leftFound, rightFound-leftFound+1);
  return result;
}

std::string StringUtil::LeftTrim(const std::string &str)
{
  using namespace std;
  int n = str.find_first_not_of("\t\n\x0b\x0c\r ");
  string left(n == string::npos ? str : str.substr(n, str.length()));
  //_Tstring left = boost::trim_left_copy(str_);
  return left;
}

std::string StringUtil::RightTrim(const std::string &str)
{
  using namespace std;
  int n = str.find_last_not_of("\t\n\x0b\x0c\r ");
  string right(n == string::npos ? str : str.substr(0, n + 1));
  //_Tstring right = boost::trim_right_copy(str_);
  return right;
}
int StringUtil::Split(const std::string &str,
  char ch, std::vector<std::string> *retval
  )
{
  using namespace std;
  SR_ASSERT(retval->empty() == true);

  //if no delimeter
  if (str.length() == 0) {
    retval->push_back(str);
    return retval->size();
  }

  //simple impl
  string tmp_token;
  for (size_t i = 0 ; i < str.length() ; i++) {
    char str_ch = str[i];
    if(str_ch == ch) {
      //split point
      retval->push_back(tmp_token);
      tmp_token.clear();
    } else {
      tmp_token.push_back(str_ch);
    }
  }
  retval->push_back(tmp_token);
  return retval->size();
}

std::string StringUtil::Join(
    const std::string &str,
    const std::vector<std::string> &tokenlist
    )
{
  using namespace std;
  ostringstream oss;
  for (size_t i = 0 ; i < tokenlist.size() ; i++) {
    oss << tokenlist[i];
    if(i != tokenlist.size() - 1) {
      oss << str;
    }
  }
  return oss.str();
}
}